#include "uvlib/subsystem.hpp"

#include "uvlib/scheduler.hpp"

namespace uvl {
void Subsystem::register_self() {
  Scheduler::get_instance().register_subsystem(this);
}

Subsystem::Subsystem() { register_self(); }

void Subsystem::initialize() {}

void Subsystem::periodic() {}

const std::optional<CommandPtr>& Subsystem::get_default_command() const {
  return m_default_command;
}

void Subsystem::set_default_command(CommandPtr command) {
  m_default_command = std::move(command);
}

void Subsystem::set_default_command(CommandPtr&& command) {
  m_default_command = std::move(command);
}
}  // namespace uvl