#include "uvlib/commands/advanced_commands/instant_command.hpp"

namespace uvl {

InstantCommand::InstantCommand(std::function<void()> callback,
                               std::initializer_list<Subsystem*> requirements)
    : m_callback(callback) {
  add_requirements(requirements);
}

void InstantCommand::execute() {
  if (!m_has_executed) {
    m_has_executed = true;
    m_callback();
  }
}

bool InstantCommand::is_finished() { return m_has_executed; }

void InstantCommand::end(bool interrupted) {}
}  // namespace uvl