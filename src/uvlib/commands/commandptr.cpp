#include "uvlib/commands/commandptr.hpp"

#include "uvlib/commands/advanced_commands/sequential_command_group.hpp"
#include "uvlib/commands/command.hpp"

namespace uvl {
CommandPtr::CommandPtr(std::unique_ptr<Command>&& command)
    : m_ptr(std::move(command)) {}

CommandPtr::CommandPtr(CommandPtr&& rhs) { m_ptr = std::move(rhs.m_ptr); }

Command* CommandPtr::operator*() { return m_ptr.get(); }

Command* CommandPtr::operator->() { return m_ptr.get(); }

CommandPtr CommandPtr::and_then(CommandPtr&& next) && {
  return SequentialCommandGroup(std::move(*this), std::move(next)).to_ptr();
}

Command* CommandPtr::get() const& { return m_ptr.get(); }

}  // namespace uvl