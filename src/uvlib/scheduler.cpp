#include "uvlib/scheduler.hpp"

#include <stdexcept>

#include "main.h"
#include "uvlib/command.hpp"
#include "uvlib/subsystem.hpp"

namespace uvlib {
/* Ensure Scheduler singleton status */
Scheduler *scheduler = nullptr;

/* Keep track of the number of ticks */
int tick_number = 0;

Scheduler &get_scheduler() {
  if (scheduler == nullptr) {
    scheduler = new Scheduler();
  }

  return *scheduler;
}

/**
 * Return true if and only if all of command's requirements
 * have not been previously used in the same tick.
 */
bool is_runnable_command(std::shared_ptr<Command> command) {
  bool safe = true;
  subsystem_list_t requirements = command->get_requirements();
  auto subsystem_it = requirements.begin();
  while (safe && subsystem_it != requirements.end()) {
    safe = !((*subsystem_it)->get_used_current_tick());
    subsystem_it++;
  }
  return safe;
}

/* Other Methods */

void Scheduler::initialize() {
  for (Subsystem *subsystem : registered_subsystems) {
    subsystem->initialize();
  }
}

void Scheduler::register_subsystem(Subsystem *subsystem) {
  registered_subsystems.push_back(subsystem);
}

commandptr_t Scheduler::schedule_command(commandptr_t command) {
  scheduled_commands.push_back(command);

  command->set_alive(true);
  command->initialize();

  return command;
}

template <typename Derived_Command, typename... Args>
constructable_command_t<Derived_Command> Scheduler::schedule_command(
    Args &&...constructor_args) {
  commandptr_t command = std::make_shared<Derived_Command>(
      std::forward<Args>(constructor_args)...);

  schedule_command(command);

  return std::static_pointer_cast<Derived_Command>(command);
}

void Scheduler::cancel_command(std::shared_ptr<Command> command) {
  command->cancel();
}

void Scheduler::mainloop_tasks() {
  /* Execute all commands */
  for (auto command_it = scheduled_commands.begin();
       command_it != scheduled_commands.end(); command_it++) {
    commandptr_t target = *command_it;

    if (!target->get_alive() || target->is_finished()) {
      if (target->get_alive()) {
        // The command successfully returned true
        // for is_finished, and therefore executed without
        // any interruption.
        target->set_alive(false);
        target->on_end(false);
      }

      // Remove the current command from the list and
      // update the iterator with the next item in the
      // list.
      command_it = scheduled_commands.erase(command_it);
    } else {
      if (is_runnable_command(target) &&
          target->get_tick_number() != tick_number) {
        // We only need to run the command if
        // has not been executed already AND
        // if all of its requirements are untouched
        // for the current tick.
        // Checking for the tick_number also removes
        // any redundantly scheduled commands.

        // Mark command as executed
        target->set_tick_number(tick_number);
        target->execute();

        // Mark all of target's requirements as used for this tick
        for (auto subsystem : target->get_requirements()) {
          subsystem->set_used_current_tick(true);
        }

        // Update iterator with the next command in the list
        command_it++;
      } else {
        if (target->get_tick_number() == tick_number) {
          // Redundantly scheduled command. Do not mark it
          // as dead it since it may still be executing.
          // We only need to remove the redundancy in the list.
        } else {
          // Required subsystems were already used
          // in the current tick. Command was interrupted.
          target->set_alive(false);
          target->on_end(true);
        }

        // Remove this command from the list
        // and update the iterator to the next command.
        command_it = scheduled_commands.erase(command_it);
      }
    }
  }

  /* Execute all subsystems and default commands */
  for (Subsystem *subsystem : registered_subsystems) {
    // Execute the default command of the subsystem if
    // and only if the current subsystem has not been
    // used already in this tick
    if (!subsystem->get_used_current_tick()) {
      std::shared_ptr<Command> default_command =
          subsystem->get_default_command();

      // Ensure the command has not already been executed
      // this tick, that all its required subsystems
      // are untouched, and that it is not finished.
      if (!default_command->is_finished() &&
          is_runnable_command(default_command) &&
          default_command->get_tick_number() != tick_number) {
        if (!default_command->get_alive()) {
          // Command has just reawakened from sleep
          default_command->initialize();
        }

        default_command->set_tick_number(tick_number);
        default_command->execute();
      } else if (default_command->get_alive()) {
        // the command was alive the previous tick but has been
        // interrupted. We need to call end() to properly clean up
        default_command->set_alive(false);
        default_command->on_end(true);
      }
    }

    subsystem->periodic();
  }

  /* Update subsystem used value back to false for next tick */
  for (Subsystem *subsystem : registered_subsystems) {
    subsystem->set_used_current_tick(false);
  }
}

void Scheduler::mainloop() {
  uint32_t now = pros::millis();
  while (true) {
    mainloop_tasks();
    pros::c::task_delay_until(&now, 20);

    // FIXME watch to ensure no issues arise
    // when this variable overflows.
    tick_number++;
  }
}

/* Getters */

const command_list_t &Scheduler::get_scheduled_commands() const {
  return scheduled_commands;
}

const subsystem_list_t &Scheduler::get_subsystems() {
  return registered_subsystems;
}
}  // namespace uvlib