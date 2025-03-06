#include "uvlib/commands/advanced_commands/sequential_command_group.hpp"

#include "uvlib/scheduler.hpp"

namespace uvl {
SequentialCommandGroup::~SequentialCommandGroup() {
  cancel_current();
  m_commands.clear();
}

void SequentialCommandGroup::schedule_current() {
  if (m_current_command != m_commands.end()) {
    // If m_current_command does not point to the last command, schedule
    // m_current_command
    Scheduler::get_instance().schedule_command(m_current_command->get());
  }
}

void SequentialCommandGroup::cancel_current() {
  if (m_current_command != m_commands.end() &&
      m_current_command->get()->is_alive()) {
    m_current_command->get()->cancel();
  }

  // Reset current command head
  m_current_command = m_commands.begin();
}

void SequentialCommandGroup::initialize() {
  // This command just got scheduled. Let's schedule the first command
  // immediately

  m_failed = false;
  m_current_command = m_commands.begin();
  schedule_current();
}

void SequentialCommandGroup::execute() {
  // Check if the current command is dead
  if (m_current_command->get()->get_state() != CommandState::kRunning) {
    if (m_current_command->get()->get_state() == CommandState::kSuccess) {
      // The command successfully returned true for is_finished, and therefore
      // executed without any interruption.

      // Continue by scheduling the next command in the list
      m_current_command++;
      schedule_current();
    } else {
      // Command unsuccessfully ended. We must cancel all remaining commands.
      cancel_current();
      m_failed = true;
    }
  }
}

void SequentialCommandGroup::end(bool interrupted) { cancel_current(); }

bool SequentialCommandGroup::is_finished() {
  return m_failed || m_current_command == m_commands.end();
}

const std::list<CommandPtr> &SequentialCommandGroup::get_commands() const {
  return m_commands;
}

const std::list<CommandPtr>::iterator &
SequentialCommandGroup::get_current_command_iterator() const {
  return m_current_command;
}
} // namespace uvl