#pragma once

#include <initializer_list>
#include <list>

#include "uvlib/enums.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
class Command {
  friend class Scheduler;

 public:
  ~Command();

  /**
   * Commands cannot be copied around; they must
   * be moved.
   */
  Command(const Command&) = delete;
  Command& operator=(const Command& rhs) = delete;

  Command(Command&&) = default;
  Command& operator=(Command&&) = default;

  /**
   * Cancels the command by setting it as not alive
   * so it is not executed in future ticks (unless
   * rescheduled) and is also removed from the
   * command_chain in the next tick.
   */
  void cancel();

  const std::list<Subsystem*>& get_requirements() const;

 protected:
  void add_requirements(std::initializer_list<Subsystem*>);

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

 private:
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

 private:
  ScheduleDirection m_scheduleDirection = ScheduleDirection::kTop;

  std::list<Subsystem*> m_requirements;

  /**
   * The last tick at which the command was executed in.
   * This is helpful to ensure no command is executed more
   * than once in a single tick.
   */
  int m_tick_number = -1;

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
  bool m_is_alive = false;
};
}  // namespace uvl