#include "uvlib/commands/advanced_commands/run_until_command.hpp"

namespace uvl {
RunUntilCommand::RunUntilCommand(
    std::function<void()> execute, std::function<bool()> is_finished,
    std::function<void(bool interrupted)> end,
    std::initializer_list<Subsystem *> requirements)
    : m_execute(std::move(execute)), m_is_finished(std::move(is_finished)),
      m_end(std::move(end)) {
  add_requirements(requirements);
}

RunUntilCommand::RunUntilCommand(
    std::function<void()> execute, std::function<bool()> is_finished,
    std::initializer_list<Subsystem *> requirements)
    : m_execute(std::move(execute)), m_is_finished(std::move(is_finished)),
      m_end([](bool interrupted) {}) {
  add_requirements(requirements);
}

void RunUntilCommand::execute() { m_execute(); }

bool RunUntilCommand::is_finished() { return m_is_finished(); }

void RunUntilCommand::end(bool interrupted) { m_end(interrupted); }
} // namespace uvl