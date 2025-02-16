#pragma once

#include <functional>

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
class FunctionCommand : public CommandHelper<Command, FunctionCommand> {
 public:
  FunctionCommand(std::function<void()> initialize,
                  std::function<void()> execute,
                  std::function<bool()> is_finished,
                  std::function<void(bool interrupted)> end);

  void initialize() override;

  void execute() override;

  bool is_finished() override;

  void end(bool interrupted) override;

 private:
  std::function<void()> m_initialize;
  std::function<void()> m_execute;
  std::function<bool()> m_is_finished;
  std::function<void(bool interrupted)> m_end;
};
}  // namespace uvl