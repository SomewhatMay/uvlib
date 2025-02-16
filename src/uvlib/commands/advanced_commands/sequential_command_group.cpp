#include "uvlib/commands/advanced_commands/sequential_command_group.hpp"

#include "uvlib/scheduler.hpp"

namespace uvl {
void SequentialCommandGroup::schedule_next() {
  if (!m_commands.empty()) {
    Scheduler::get_instance().schedule_command(m_commands.front().get());
  }
}

void SequentialCommandGroup::initialize() {
  // This command just got scheduled. Let's schedule the first command
  // immediately
  schedule_next();
}

void SequentialCommandGroup::execute() {
  // Check if the current command is dead
  if (!m_commands.front()->is_alive()) {
    if (m_commands.front()->is_finished()) {
      // The command successfully returned true for is_finished, and therefore
      // executed without any interruption.

      // FIXME ensure this calls the CommandPtr's destructor
      m_commands.pop_front();

      // Continue by scheduling the next command in the list
      schedule_next();
    } else {
      // Command unsuccessfully ended. We must cancel ourselves.
      cancel();
    }
  }
}

bool SequentialCommandGroup::is_finished() { return m_commands.empty(); }
}  // namespace uvl