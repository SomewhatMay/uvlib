#pragma once

#include <initializer_list>
#include <list>

#include "uvlib/enums.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * Any precise set of instructions the robot should take, given a specific
 * event.
 *
 * Ex. EnableElevator, DriveForward, LiftArm, etc.
 */
class Command {
  friend class Scheduler;
  friend class Trigger;

public:
  Command() = default;

  virtual ~Command();

  /**
   * Commands cannot be copied around; they must
   * be moved.
   */
  Command(const Command &) = delete;
  Command &operator=(const Command &rhs) = delete;

  Command(Command &&) = default;
  Command &operator=(Command &&) = default;

  /**
   * Cancels the command by setting it as not alive so it is not executed in
   * future ticks (unless rescheduled) and is also removed from the
   * command_chain in the next tick.
   */
  void cancel();

  /**
   * Chains next with this command using the SequentialCommandGroup command.
   * Next is guaranteed to schedule when and if this command finishes
   * successfully.
   *
   * @note This invalidates the current command and converts it to a CommandPtr,
   * identical to calling Command.to_ptr().and_then(next);
   *
   * @param next The command to schedule after this one.
   *
   * @return The SequentialCommandGroup CommandPtr.
   */
  virtual CommandPtr and_then(CommandPtr &&next) &&;

  /**
   * Every command has a set of subsystem requirements. These subsystems must be
   * free for the command to execute. This ensures two commands do not try to
   * change the state of a singular subsystem at the same time, which may lead
   * to unexpected behaviour.
   */
  const std::list<Subsystem *> &get_requirements() const;

  /**
   * Returns true if the command is currently alive, false otherwise.
   *
   * @note used internally to check whether the command is still in the
   * scheduler command list.
   */
  const bool &is_alive() const;

  /**
   * Return true if and only if the command has achieved its goal.
   *
   * @note This method must return true if the command is finished, regardless
   * of how many times it is called consecutively. Do not code this method in a
   * way that it only returns true once if the command is finished.
   *
   * @note It is generally a good idea to keep all actions inside this method to
   * be short and read-only operations. If the command is finished, it is
   * strongly encouraged to move the end behaviour to Command::end().
   *
   * @warning This method should not assign any values inside any subsystems.
   * Doing so can cause undefined behaviour.
   */
  virtual bool is_finished();

  /**
   * Moves this command to a CommandPtr object and transfers ownership. This
   * ensures that memory ownership is managed appropriately.
   *
   * @warning This method invalidates the current command through a move
   * operation.
   *
   * @note This method is automatically overriden by the uvl::CommandHelper
   * class and does not need any user configuration. It is recommended to
   * inherit from the uvl::CommandHelper class when creating a custom command
   * class.
   */
  virtual CommandPtr to_ptr() && = 0;

protected:
  /**
   * Appends to the list of subsystem requirements.
   *
   * To learn more,
   * @see uvl::Command::get_requirements()
   */
  void add_requirements(std::initializer_list<Subsystem *>);

  /**
   * Implement any initialization logic here. Since commands are reusable and
   * are only instantiated once, this method can be used to properly initialize
   * any internal state before Command::execute() is called.
   */
  virtual void initialize();

  /**
   * Periodically executed by the Scheduler at intervals
   * before any subsystems are updated.
   */
  virtual void execute();

  /**
   * Called when a command is either finished (determined by
   * Command::is_finished()), or when it is interrupted (Command::cancel() or
   * another resource requires the subsystems). The interrupted parameter is
   * false if and only if the command is ended due a true return from
   * Command::is_finished().
   *
   * Automatically called when the command is ended by
   * the on_end(bool) member function.
   *
   * @warning Users should NEVER call this method directly; use the
   * Command::cancel() method instead.
   */
  virtual void end(bool interrupted);

  /**
   * Determines where in the command list the scheduler will place this command.
   * Very helpful for command groups since all their children commands need to
   * execute before the parent command can properly query their children.
   */
  ScheduleDirection m_schedule_direction = ScheduleDirection::kTop;

private:
  /**
   * The method called by the scheduler when a command is ended. This handles
   * internal actions that must happen when the command comes to an end, and
   * will call end(bool) on its own when possible.
   *
   * @note end(bool) is always called before any and_then, catch, or finally
   * commands are scheduled.
   */
  void on_end(bool interrupted);

  std::list<Subsystem *> m_requirements;

  /**
   * The last tick at which the command was executed in.
   * This is helpful to ensure no command is executed more
   * than once in a single tick.
   */
  int m_tick_number = -1;

  /**
   * Whether the command has been scheduled to be executed for the next tick or
   * not.
   *
   * The value is automatically assigned as true by the scheduler when the
   * command is scheduled or rescheduled.
   *
   * @note If this value is true, the command is not guaranteed to run, rather,
   * it is only guaranteed to considered by the scheduler. It may not run if
   * multiple versions of this command is scheduled or this command requires
   * another subsystem that has already been used by another command in the same
   * tick.
   */
  bool m_is_alive = false;
};
} // namespace uvl