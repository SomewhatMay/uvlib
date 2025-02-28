#pragma once

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/commands/commandptr.hpp"
#include "uvlib/typedefs.hpp"
#include <list>

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

  template <typename... Args> SequentialCommandGroup(Args &&...args) {
    static_assert(
        (std::is_same_v<std::remove_reference_t<Args>, CommandPtr> && ...),
        "All arguments must be of type CommandPtr");

    // Move each argument into the vector
    (m_commands.push_front(std::forward<Args>(args)), ...);
  }

  SequentialCommandGroup(SequentialCommandGroup &&) = default;
  SequentialCommandGroup &operator=(SequentialCommandGroup &&) = default;

  ~SequentialCommandGroup() override;

  /**
   * @brief Cancels the command scheduled by this SequentialCommandGroup
   * currently, if one exists, and is currently scheduled.
   */
  void cancel_current();

  /**
   * @brief Get all the commands (owned) and handled by this
   * SequentialCommandGroup.
   *
   * @return const std::list<CommandPtr>&
   */
  const std::list<CommandPtr> &get_commands() const;

  /**
   * @brief Get a readonly reference to the currently scheduled command's
   * iterator. Iterator validity is not guaranteed.
   *
   * @return const std::list<CommandPtr>::iterator&
   */
  const std::list<CommandPtr>::iterator &get_current_command_iterator() const;

protected:
  void initialize() override;

  void execute() override;

  bool is_finished() override;

  void end(bool interrupted) override;

private:
  std::list<CommandPtr> m_commands;

  /**
   * @brief The current command that is scheduled by the SequentialCommandGroup.
   */
  std::list<CommandPtr>::iterator m_current_command;

  /**
   * @brief Whether any commands in the chain have failed since the last time
   * this SequentialCommandGroup was scheduled.
   *
   * As soon as the currently scheduled command is interrupted, this value is
   * set to true until the next time this SequentialCommandGroup is scheduled.
   *
   */
  bool m_failed = false;

  void schedule_current();
};
} // namespace uvl