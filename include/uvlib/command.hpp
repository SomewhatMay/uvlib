#pragma once

#include <optional>
#include <vector>

#include "uvlib/typedefs.hpp"

namespace uvlib {
class Command {
 private:
  ScheduleDirection scheduleDirection = ScheduleDirection::TOP;

  std::list<Subsystem*> requirements;

  std::optional<std::list<commandptr_t>> and_then_commands;
  std::optional<std::list<commandptr_t>> finally_commands;

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

  virtual void end(bool interrupted);

  /**
   * Cancels the command by setting it as not alive
   * so it is not executed in future ticks (unless
   * rescheduled) and is also removed from the
   * command_chain in the next tick.
   */
  void cancel();

  /* Getters and Setters */

  const std::list<Subsystem*>& get_requirements() const;

  bool get_alive() const;

  void set_alive(bool alive);

  /**
   * Internal method.
   */
  void set_tick_number(int tick_number);

  int get_tick_number() const;
};

}  // namespace uvlib