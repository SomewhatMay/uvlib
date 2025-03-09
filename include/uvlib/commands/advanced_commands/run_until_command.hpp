#pragma once

#include <functional>
#include <initializer_list>

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/subsystem.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 */
class RunUntilCommand : public CommandHelper<Command, RunUntilCommand> {
public:
  RunUntilCommand(std::function<void()> execute,
                  std::function<bool()> is_finished,
                  std::function<void(bool interrupted)> end,
                  std::initializer_list<Subsystem *> requirements);

  RunUntilCommand(std::function<void()> execute,
                  std::function<bool()> is_finished,
                  std::initializer_list<Subsystem *> requirements);

  bool is_finished() override;

protected:
  void execute() override;

  void end(bool interrupted) override;

private:
  std::function<void()> m_execute;
  std::function<bool()> m_is_finished;
  std::function<void(bool interrupted)> m_end;
};
} // namespace uvl