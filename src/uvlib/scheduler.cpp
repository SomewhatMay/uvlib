#include "uvlib/scheduler.hpp"

#include <stdexcept>
#include <unordered_set>

#include "main.h"
#include "uvlib/commands/command.hpp"
#include "uvlib/commands/commandptr.hpp"
#include "uvlib/subsystem.hpp"

namespace uvl {
/* Keep track of the number of ticks */
int tick_number = 0;

/* Other Methods */

void Scheduler::initialize() {
  for (Subsystem *subsystem : m_registered_subsystems) {
    subsystem->initialize();
  }
}

void Scheduler::register_subsystem(Subsystem *subsystem) {
  if (std::find(m_registered_subsystems.begin(), m_registered_subsystems.end(),
                subsystem) != m_registered_subsystems.end()) {
    throw std::runtime_error("Subsystem already registered");
  }

  m_registered_subsystems.push_back(subsystem);
}

bool Scheduler::schedule_command(Command *command) {
  // Dont schedule the same command twice
  if (std::find(m_scheduled_commands.begin(), m_scheduled_commands.end(),
                command) != m_scheduled_commands.end()) {
    return false;
  }

  const auto &requirements = command->get_requirements();

  std::unordered_set<Command *> conflicting_cmds;
  conflicting_cmds.reserve(m_active_subsystems.size() / 2);

  for (auto subsystem : requirements) {
    auto active_command = m_active_subsystems.find(subsystem);
    if (active_command != m_active_subsystems.end()) {
      conflicting_cmds.insert(active_command->second);
    }
  }

  for (auto active_command : conflicting_cmds) {
    // This should also remove this command's requirements
    // from the m_active_subsystems map
    cancel_command(active_command);
  }

  // Mark each requirement as active with the current command
  for (auto subsystem : requirements) {
    m_active_subsystems[subsystem] = command;
  }

  if (command->m_schedule_direction == ScheduleDirection::kTop) {
    m_scheduled_commands.push_front(command);
  } else if (command->m_schedule_direction == ScheduleDirection::kBottom) {
    m_scheduled_commands.push_back(command);
  }

  command->m_is_alive = true;
  command->initialize();

  return true;
}

bool Scheduler::schedule_command(CommandPtr &&command) {
  Command *raw_ptr = command.get();
  if (schedule_command(raw_ptr)) {
    // Only accept ownership if the command was successfully scheduled
    m_owned_commands.insert({raw_ptr, std::move(command)});

    return true;
  }

  return false;
}

void Scheduler::cancel_command(Command *command) {
  if (command->m_is_alive) {
    command->m_is_alive = false;

    // Remove the requirements from the active subsystems map
    for (auto subsystem : command->get_requirements()) {
      m_active_subsystems.erase(subsystem);
    }

    auto owned_command = m_owned_commands.find(command);
    if (owned_command != m_owned_commands.end()) {
      // FIXME ensure this does call the destructor of the CommandPtr
      m_owned_commands.erase(owned_command);
    }

    m_scheduled_commands.remove(command);
    command->on_end(true);
  }
}

void Scheduler::cancel_command(CommandPtr &&command) {
  cancel_command(command.get());
}

void Scheduler::run() {
  /* Execute all commands */
  for (auto command_it = m_scheduled_commands.begin();
       command_it != m_scheduled_commands.end(); command_it++) {
    Command *target = *command_it;

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
      command_it = m_scheduled_commands.erase(command_it);
    } else {
      if (target->m_tick_number != tick_number) {
        // We only need to run the command if
        // has not been executed already, ensuring
        // duplicated commands are not executed.

        // Mark command as executed
        target->m_tick_number = tick_number;
        target->execute();

        // Update iterator with the next command in the list
        command_it++;
      } else {
        // Redundantly scheduled command. Do not mark it
        // as dead it since it may still be executing.
        // Remove this command from the list
        // and update the iterator to the next command.
        command_it = m_scheduled_commands.erase(command_it);
      }
    }
  }

  /* Execute all subsystems and default commands */
  for (Subsystem *subsystem : m_registered_subsystems) {
    // Execute the default command of the subsystem if
    // and only if no other commands have this subsystem as
    // a requirement.
    if (m_active_subsystems.contains(subsystem)) {
      std::optional<CommandPtr> &default_command_container =
          subsystem->m_default_command;

      if (default_command_container) {
        CommandPtr &default_command = *default_command_container;
        // Ensure the command has not already been executed
        // this tick, that all its required subsystems
        // are untouched, and that it is not finished.

        bool is_runnable_command = true;
        for (auto requirement : default_command->get_requirements()) {
          is_runnable_command =
              is_runnable_command && !m_active_subsystems.contains(requirement);

          if (!is_runnable_command) break;
        }

        if (!default_command->is_finished() && is_runnable_command &&
            default_command->m_tick_number != tick_number) {
          if (!default_command->m_is_alive) {
            // Command has just reawakened from being dead in the previous tick
            default_command->initialize();
          }

          default_command->m_tick_number = tick_number;
          default_command->execute();
        } else if (default_command->m_is_alive) {
          // the command was alive the previous tick but has been
          // interrupted. We need to call on_end() to properly clean up.
          default_command->m_is_alive = false;
          default_command->on_end(true);
        }
      }
    }

    subsystem->periodic();
  }
}

void Scheduler::mainloop() {
  uint32_t now = pros::millis();
  while (true) {
    run();
    pros::c::task_delay_until(&now, 20);

    // FIXME watch to ensure no issues arise
    // when this variable overflows.
    tick_number++;
  }
}

/* Getters */

const std::list<Command *> &Scheduler::get_scheduled_commands() const {
  return m_scheduled_commands;
}

const std::unordered_map<Command *, CommandPtr> &Scheduler::get_owned_commands()
    const {
  return m_owned_commands;
}

const std::list<Subsystem *> &Scheduler::get_subsystems() {
  return m_registered_subsystems;
}
}  // namespace uvl