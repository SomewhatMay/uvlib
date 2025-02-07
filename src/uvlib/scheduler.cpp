#include "uvlib/scheduler.hpp"

#include <stdexcept>

#include "main.h"

namespace uvlib {
/* Ensure Scheduler singleton status */
Scheduler *scheduler = nullptr;

const Scheduler &get_scheduler() {
  if (scheduler == nullptr) {
    scheduler = new Scheduler();
  }

  return *scheduler;
}

/* Other Methods */

void Scheduler::initialize() {
  for (Subsystem *subsystem : registered_subsystems) {
    subsystem->initialize();
  }
}

void Scheduler::mainloop_tasks() {
  /* Execute all subsystems */

  for (Subsystem *subsystem : registered_subsystems) {
    subsystem->periodic();
  }

  /* Execute all commands */
  for (auto command_chain = scheduled_commands.begin();
       command_chain != scheduled_commands.end();) {
    if (command_chain->empty()) {
      // Remove chain from list and get the command
      // chain right after this one
      command_chain = scheduled_commands.erase(command_chain);
    } else {
      Command *target = command_chain->front();

      if (target->is_finished()) {
        target->end(false);
        // Remove this command and rerun the loop on this
        // same chain to potentially run the next command
        command_chain->pop_front();
      } else {
        target->execute();
        // Execute the current front command and
        // get the next command chain
        command_chain++;
      }
    }
  }
}

void Scheduler::mainloop() {
  uint32_t now = pros::millis();
  while (true) {
    mainloop_tasks();
    pros::c::task_delay_until(&now, 20);
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