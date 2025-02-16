#pragma once

#include <bitset>
#include <concepts>
#include <list>
#include <memory>
#include <stack>
#include <unordered_map>

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
   * Schedule a command to be considered by the
   * scheduler for the next tick.
   *
   * @returns true if the command was successfully scheduled.
   */
  bool schedule_command(CommandPtr &&command);

  /**
   * Schedule a command to be considered by the
   * scheduler for the next tick.
   *
   * @note Command must not be destroyed until it is
   *        either interrupted or finished.
   *
   * @warning this method is to be used internally,
   *          and should not be called directly.
   *          Overusing of this method directly may
   *          lead to undefined behaviour.
   *
   * @returns true if the command was successfully scheduled.
   */
  bool schedule_command(Command *command);

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
  const std::list<Command *> &get_scheduled_commands() const;

  /**
   * Get a const reference to the commands
   * owned by the scheduler. These commands
   * will be automatically destroyed by the
   * scheduler when they are no longer alive.
   */
  const std::unordered_map<Command *, CommandPtr> &get_owned_commands() const;

  /**
   * Get a list of every subsystem that has been
   * successfully registered to the scheduler.
   */
  const std::list<Subsystem *> &get_subsystems();

  /**
   * Get a const reference to the subsystems
   * that are being used by the scheduled commands.
   */
  const std::unordered_map<Subsystem *, Command *> &get_active_subsystems()
      const;

 private:
  friend class Singleton;
  friend class Subsystem;

  Scheduler() = default;

  std::list<Command *> m_scheduled_commands;
  std::list<Subsystem *> m_registered_subsystems;

  /**
   * Represents the subsystems that are being used by the
   * scheduled commands.
   */
  std::unordered_map<Subsystem *, Command *> m_active_subsystems;

  /**
   * Commands owned by the scheduler. These
   * commands will be automatically destroyed by
   * the scheduler when they are no longer alive.
   */
  std::unordered_map<Command *, CommandPtr> m_owned_commands;

  /**
   * Internal method only: to be used by the subsystem superclass
   * automatically.
   */
  void register_subsystem(Subsystem *subsystem);

  /**
   * Runs an iteration ("a tick") of the scheduler, executing all
   * scheduled commands and registered subsystem periodic methods.
   *
   * @note This is an internal method and is automatically called
   * by the Scheduler::mainloop() method.
   */
  void run();
};
}  // namespace uvl