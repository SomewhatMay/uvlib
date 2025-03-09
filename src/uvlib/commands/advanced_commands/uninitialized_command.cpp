#include "uvlib/commands/advanced_commands/uninitialized_command.hpp"

namespace uvl {
UninitializedCommand::UninitializedCommand(
    std::function<void()> execute, std::function<bool()> is_finished,
    std::function<void(bool interrupted)> end,
    std::initializer_list<Subsystem *> requirements)
    : m_execute(std::move(execute)), m_is_finished(std::move(is_finished)),
      m_end(std::move(end)) {
  add_requirements(requirements);
}

void UninitializedCommand::execute() { m_execute(); }

bool UninitializedCommand::is_finished() { return m_is_finished(); }

void UninitializedCommand::end(bool interrupted) { m_end(interrupted); }
} // namespace uvl