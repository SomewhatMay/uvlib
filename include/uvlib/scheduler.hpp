#pragma once

#include <bitset>
#include <concepts>
#include <list>
#include <memory>
#include <stack>

#include "uvlib/commands/commandptr.hpp"
#include "uvlib/singleton.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {

/**
 * WARNING: This class should not be manually instantiated. Instead,
 * use the Scheduler::get_instance() method instead.
 */
class Scheduler : public Singleton<Scheduler> {
 public:
  /**
   * To get a reference to the active scheduler
   * in the current environment, use the following
   * command:
   *
   * static T& Scheduler::get_instance();
   *
   * This command creates a new scheduler the first
   * time it is called.
   *
   * NOTE: Always use this method instead of instantiating
   * a new Scheduler object.
   */

  /**
   * Calls initialize on all registered subsystems.
   *
   * NOTE: Only call this method once you have instantiated
   * every subsystem. Instantiating any subsystems after
   * calling this method will leave them uninitialized.
   */
  void initialize();

  /**
   * Does not run on a separate task. If mainloop is
   * called in a separate task, ensure that critical
   * variables are atomic.
   */
  void mainloop();

  /**
   * Internal method only: to be used by the subsystem superclass
   * automatically.
   */
  void register_subsystem(Subsystem *subsystem);

  /**
   * Schedule a command to be considered by the
   * scheduler for the next tick.
   */
  void schedule_command(CommandPtr &&command);

  /**
   * Mark command as dead, preventing it from
   * being executed again in the next tick and
   * ultimately removed from the execution stack.
   */
  void cancel_command(CommandPtr &&command);

  /**
   * @see cancel_command(CommandPtr &&command)
   */
  void cancel_command(Command *command);

  /**
   * Get a const reference to the commands
   * currently scheduled by the scheduler.
   * Not every scheduled command is guaranteed
   * to execute.
   */
  const std::list<CommandPtr> &get_scheduled_commands() const;

  /**
   * Get a list of every subsystem that has been
   * successfully registered to the scheduler.
   */
  const std::list<Subsystem *> &get_subsystems();

 private:
  std::list<CommandPtr> scheduled_commands;
  std::list<Subsystem *> registered_subsystems;

  void mainloop_tasks();
};
}  // namespace uvl