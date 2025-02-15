#include "uvlib/scheduler.hpp"

#include <stdexcept>

#include "main.h"
#include "uvlib/commands/command.hpp"
#include "uvlib/commands/commandptr.hpp"
#include "uvlib/subsystem.hpp"

namespace uvl {
/* Keep track of the number of ticks */
int tick_number = 0;

/**
 * Return true if and only if all of command's requirements
 * have not been previously used in the same tick.
 */
bool is_runnable_command(Command *command) {
  bool safe = true;
  std::list<Subsystem *> requirements = command->get_requirements();
  auto subsystem_it = requirements.begin();
  while (safe && subsystem_it != requirements.end()) {
    safe = !((*subsystem_it)->m_used_current_tick);
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

void Scheduler::schedule_command(CommandPtr &&command) {
  command->m_is_alive = true;
  command->initialize();

  // invalidates command, therefore must happen last
  scheduled_commands.push_back(std::move(command));
}

void Scheduler::cancel_command(CommandPtr &&command) {
  if (command->m_is_alive) {
    command->m_is_alive = false;
    command->on_end(true);
  }
}

void Scheduler::cancel_command(Command *command) {
  if (command->m_is_alive) {
    command->m_is_alive = false;
    command->on_end(true);
  }
}

void Scheduler::mainloop_tasks() {
  /* Execute all commands */
  for (auto command_it = scheduled_commands.begin();
       command_it != scheduled_commands.end(); command_it++) {
    CommandPtr &target = *command_it;

    if (!target->m_is_alive || target->is_finished()) {
      if (target->m_is_alive) {
        // The command successfully returned true
        // for is_finished, and therefore executed without
        // any interruption.
        target->m_is_alive = false;
        target->on_end(false);
      }

      // Remove the current command from the list and
      // update the iterator with the next item in the
      // list.
      command_it = scheduled_commands.erase(command_it);
    } else {
      if (is_runnable_command(target.get()) &&
          target->m_tick_number != tick_number) {
        // We only need to run the command if
        // has not been executed already AND
        // if all of its requirements are untouched
        // for the current tick.
        // Checking for the tick_number also removes
        // any redundantly scheduled commands.

        // Mark command as executed
        target->m_tick_number = tick_number;
        target->execute();

        // Mark all of target's requirements as used for this tick
        for (auto subsystem : target->get_requirements()) {
          subsystem->m_used_current_tick = true;
        }

        // Update iterator with the next command in the list
        command_it++;
      } else {
        if (target->m_tick_number == tick_number) {
          // Redundantly scheduled command. Do not mark it
          // as dead it since it may still be executing.
          // We only need to remove the redundancy in the list.
        } else {
          // Required subsystems were already used
          // in the current tick. Command was interrupted.
          target->m_is_alive = false;
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
    if (!subsystem->m_used_current_tick) {
      std::optional<CommandPtr> &default_command_container =
          subsystem->m_default_command;

      if (default_command_container) {
        CommandPtr &default_command = *default_command_container;
        // Ensure the command has not already been executed
        // this tick, that all its required subsystems
        // are untouched, and that it is not finished.
        if (!default_command->is_finished() &&
            is_runnable_command(default_command.get()) &&
            default_command->m_tick_number != tick_number) {
          if (!default_command->m_is_alive) {
            // Command has just reawakened from sleep
            default_command->initialize();
          }

          default_command->m_tick_number = tick_number;
          default_command->execute();
        } else if (default_command->m_is_alive) {
          // the command was alive the previous tick but has been
          // interrupted. We need to call end() to properly clean up
          default_command->m_is_alive = false;
          default_command->on_end(true);
        }
      }
    }

    subsystem->periodic();
  }

  /* Update subsystem used value back to false for next tick */
  for (Subsystem *subsystem : registered_subsystems) {
    subsystem->m_used_current_tick = false;
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

const std::list<CommandPtr> &Scheduler::get_scheduled_commands() const {
  return scheduled_commands;
}

const std::list<Subsystem *> &Scheduler::get_subsystems() {
  return registered_subsystems;
}
}  // namespace uvl