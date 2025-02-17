#include "uvlib/commands/advanced_commands/wait_until_command.hpp"
#include "pros/rtos.hpp"
#include "uvlib/subsystem.hpp"
#include <functional>
#include <initializer_list>

namespace uvl {
WaitUntilCommand::WaitUntilCommand(
    std::function<bool()> condition, double duration,
    std::initializer_list<Subsystem *> requirements)
    : m_condition(std::move(condition)),
      m_timeout_end(pros::millis() + duration) {
  add_requirements(requirements);
}

bool WaitUntilCommand::is_finished() {
  return m_condition() || pros::millis() >= m_timeout_end;
}
} // namespace uvl