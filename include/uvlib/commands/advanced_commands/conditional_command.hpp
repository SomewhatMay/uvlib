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
class ConditionalCommand : public CommandHelper<Command, ConditionalCommand> {
public:
  ConditionalCommand(std::function<bool()> conditional, CommandPtr &&if_true,
                     CommandPtr &&if_false);

  bool is_finished() override;

protected:
  void initialize() override;

  void end(bool interrupted) override;

private:
  std::function<bool()> m_conditional;
  CommandPtr m_if_true;
  CommandPtr m_if_false;

  bool m_executed = false;
};
} // namespace uvl