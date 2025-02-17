#pragma once

#include <functional>
#include <initializer_list>

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/subsystem.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * An entire command, in the form of pure lambda functions. When you need
 * things done quickly and dirty. This command internally creates a command that
 * has all the regular command member functions such as initialize(), execute(),
 * is_finished(), and even end().
 */
class FunctionCommand : public CommandHelper<Command, FunctionCommand> {
 public:
  FunctionCommand(std::function<void()> initialize,
                  std::function<void()> execute,
                  std::function<bool()> is_finished,
                  std::function<void(bool interrupted)> end,
                  std::initializer_list<Subsystem*> requirements);

  bool is_finished() override;

 protected:
  void initialize() override;

  void execute() override;

  void end(bool interrupted) override;

 private:
  std::function<void()> m_initialize;
  std::function<void()> m_execute;
  std::function<bool()> m_is_finished;
  std::function<void(bool interrupted)> m_end;
};
}  // namespace uvl