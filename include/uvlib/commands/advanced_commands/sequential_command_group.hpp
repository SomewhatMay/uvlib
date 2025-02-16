#pragma once

#include <forward_list>

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/commands/commandptr.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {

/**
 * Start at the first command and execute each command after it one by one if
 * and only if its previous command successfully finishes (i.e. does not get
 * interrupted or cancelled).
 *
 * Ex. If two commands are added to a SequentialCommandGroup, the first command
 * will execute first and then the second command will execute after it if and
 * only if the first command successfully finishes.
 */
class SequentialCommandGroup
    : public CommandHelper<Command, SequentialCommandGroup> {
 public:
  /**
   * @param commands The list of commands to be executed in order. The first
   * command is instantly scheduled when this command group is scheduled.
   */
  template <typename... Args>
  SequentialCommandGroup(Args&&... args) {
    static_assert(
        (std::is_same_v<std::remove_reference_t<Args>, CommandPtr> && ...),
        "All arguments must be of type CommandPtr");

    // Move each argument into the vector
    (m_commands.push_front(std::forward<Args>(args)), ...);
  }

 protected:
  void initialize() override;

  void execute() override;

  bool is_finished() override;

 private:
  std::forward_list<CommandPtr> m_commands;

  void schedule_next();
};
}  // namespace uvl