#include "uvlib/scheduler.hpp"

#include <stdexcept>

namespace uvlib {
/* Ensure Scheduler singleton status */
bool scheduler_created = false;

/* Constructors */

Scheduler::Scheduler() {
  if (scheduler_created) {
    throw std::runtime_error("Scheduler instance already created");
  }
  scheduler_created = true;
}

/* Other Methods */

void Scheduler::initialize() {
  for (Subsystem subsystem : registered_subsystems) {
    // subsystem.initialize();
  }
}

/* Getters */

const command_list_t &Scheduler::get_scheduled_commands() {
  return scheduled_commands;
}

const subsystem_list_t &Scheduler::get_subsystems() {
  return registered_subsystems;
}
}  // namespace uvlib