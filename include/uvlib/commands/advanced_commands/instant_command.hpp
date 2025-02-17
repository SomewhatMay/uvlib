#pragma once

#include <functional>
#include <initializer_list>

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * A command that initializes, executes once, finishes, then gets removed by the
 * scheduler on the very next tick. Since InstantCommands have requirements,
 * they are able to interrupt other scheduled commands.
 */
class InstantCommand : public CommandHelper<Command, InstantCommand> {
 public:
  /**
   * @param callback The callback to be executed as if it were the 'execute()'
   * member function of a command. It should contain the code you need to run
   * once.
   *
   * @param requirements Subsystems that this command will modify.
   */
  InstantCommand(std::function<void()> callback,
                 std::initializer_list<Subsystem*> requirements);

  bool is_finished() override;

 protected:
  void execute() override;

  void end(bool interrupted) override;

 private:
  bool m_has_executed = false;
  std::function<void()> m_callback;
};
}  // namespace uvl