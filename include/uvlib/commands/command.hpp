#pragma once

#include <forward_list>
#include <optional>
#include <vector>

#include "uvlib/typedefs.hpp"

namespace uvl {
class Command {
 private:
  ScheduleDirection scheduleDirection = ScheduleDirection::kTop;

  std::list<Subsystem*> requirements;

  std::optional<std::forward_list<commandptr_t>> and_then_commands;
  std::optional<std::forward_list<commandptr_t>> on_interrupted_commands;
  std::optional<std::forward_list<commandptr_t>> finally_commands;

  /**
   * The last tick at which the command was executed in.
   * This is helpful to ensure no command is executed more
   * than once in a single tick.
   */
  int tick_number = -1;

  /**
   * Whether the command has been scheduled to be executed
   * for the next tick or not.
   *
   * The value is automatically assigned as true by
   * the scheduler when the command is scheduled
   * or rescheduled.
   *
   * NOTE: If this value is true, the command is not
   * guaranteed to run, rather, it is only guaranteed to
   * considered by the scheduler. It may not run if multiple
   * versions of this command is scheduled or this command
   * requires another subsystem that has already been used
   * by another command in the same tick.
   */
  bool is_alive = false;

 protected:
  void set_requirements(const std::initializer_list<Subsystem*>& requirements);

 public:
  virtual ~Command() = default;

  virtual void initialize();

  virtual void execute();

  /**
   * It is generally a good idea to keep all actions inside
   * this method to be short and read-only operations. If
   * the command is finished, it is strongly encouraged to
   * move the finished logic to end().
   *
   * WARNING: This method should not assign any values inside
   * any subsystems. Doing so can cause undefined behaviour.
   */
  virtual bool is_finished() = 0;

  /**
   * Automatically called when the command is ended by
   * the on_end(bool) member function.
   *
   * WARNING: Users should NEVER call this method directly;
   * use the Command::cancel() method instead.
   */
  virtual void end(bool interrupted);

  /**
   * The method called by the scheduler when a
   * command is ended. This handles internal
   * actions that must happen when the command comes
   * to an end, and will call end(bool) on its own
   * when possible.
   *
   * NOTE: end(bool) is always called before any and_then,
   * catch, or finally commands are scheduled.
   */
  void on_end(bool interrupted);

  /**
   * Cancels the command by setting it as not alive
   * so it is not executed in future ticks (unless
   * rescheduled) and is also removed from the
   * command_chain in the next tick.
   */
  void cancel();

  /* Command Composition */

  /* composition: and_then */
  cmdptr<Command> and_then(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  cmdptr<DerivedCommand> and_then(Args&&... constructor_args);

  /* composition: on_interrupted */
  cmdptr<Command> on_interrupted(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  cmdptr<DerivedCommand> on_interrupted(Args&&... constructor_args);

  /* composition: finally */
  cmdptr<Command> finally(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  cmdptr<DerivedCommand> finally(Args&&... constructor_args);

  /* Getters and Setters */

  const std::list<Subsystem*>& get_requirements() const;

  bool get_alive() const;

  void set_alive(bool alive);

  ScheduleDirection get_schedule_direction() const;

  void set_schedules_direction(ScheduleDirection direction);

  /**
   * Internal method.
   */
  void set_tick_number(int tick_number);

  int get_tick_number() const;
};

template <typename DerivedCommand, typename... Args>
cmdptr<DerivedCommand> mkcmd(Args&&... constructor_args);
}  // namespace uvl