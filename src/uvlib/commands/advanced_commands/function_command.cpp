#include "uvlib/commands/advanced_commands/function_command.hpp"

namespace uvl {
FunctionCommand::FunctionCommand(std::function<void()> initialize,
                                 std::function<void()> execute,
                                 std::function<bool()> is_finished,
                                 std::function<void(bool interrupted)> end)
    : m_initialize(std::move(initialize)),
      m_execute(std::move(execute)),
      m_is_finished(std::move(is_finished)),
      m_end(std::move(end)) {}

void FunctionCommand::initialize() { m_initialize(); }

void FunctionCommand::execute() { m_execute(); }

bool FunctionCommand::is_finished() { return m_is_finished(); }

void FunctionCommand::end(bool interrupted) { m_end(interrupted); }
}  // namespace uvl