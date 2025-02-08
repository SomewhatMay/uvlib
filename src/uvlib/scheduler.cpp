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

void Scheduler::mainloop_tasks() {
  /* Execute all commands */
  for (auto command_chain = scheduled_commands.begin();
       command_chain != scheduled_commands.end();) {
    if (command_chain->empty()) {
      // Remove chain from list and get the command
      // chain right after this one
      command_chain = scheduled_commands.erase(command_chain);
    } else {
      std::shared_ptr<Command> target = command_chain->front();

      if (target->is_finished()) {
        // Remove this command and rerun the loop on this
        // same chain to potentially run the next command.
        target->end(false);
        command_chain->pop_front();
      } else if (target->get_tick_number() != tick_number) {
        // Only run the command if it has not already ran yet
        // (avoids double-execution).

        // Ensure that the command's requirements have
        // not already been used.
        if (is_runnable_command(target)) {
          // Mark command as executed.
          target->set_tick_number(tick_number);
          target->execute();

          // Set all of target's requirements as used for this tick
          for (auto subsystem : target->get_requirements()) {
            subsystem->set_used_current_tick(true);
          }

          // Get the next command chain as this chain's front
          // has been successfully executed.
          command_chain++;
        } else {
          // command was interrupted
          target->end(true);

          // Remove this command and rerun the loop on this
          // same chain to potentially run the next command.
          command_chain->pop_front();

          // No need to update command chain; we should first
          // check if the next command in line can be safely
          // executed since this current one was interrupted.
        }
      }
    }
  }

  /* Execute all subsystems */
  for (Subsystem *subsystem : registered_subsystems) {
    // Execute the default command of the subsystem if
    // and only if the current subsystem has not been
    // used already in this tick
    if (!subsystem->get_used_current_tick()) {
      std::shared_ptr<Command> default_command =
          subsystem->get_default_command();

      // Ensure the command has not already been executed
      //  this tick and that all its required subsystems have been untouched
      if (is_runnable_command(default_command) &&
          default_command->get_tick_number() != tick_number) {
        default_command->set_tick_number(tick_number);
        default_command->execute();
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