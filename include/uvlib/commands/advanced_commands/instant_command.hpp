#pragma once

#include <functional>
#include <initializer_list>

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
class InstantCommand : public CommandHelper<Command, InstantCommand> {
 public:
  InstantCommand(std::function<void()> callback,
                 std::initializer_list<Subsystem*> requirements);

  void execute() override;

  bool is_finished() override;

  void end(bool interrupted) override;

 private:
  bool m_has_executed = false;
  std::function<void()> m_callback;
};
}  // namespace uvl