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
   * Call this method after you have registered
   * every subsystem.
   *
   * NOTE: Registering any subsystems after calling
   * the method below will leave them uninitialized.
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

  void schedule_command(CommandPtr &&command);

  void cancel_command(CommandPtr &&command);

  void cancel_command(Command *command);

  const std::list<CommandPtr> &get_scheduled_commands() const;

  const std::list<Subsystem *> &get_subsystems();

 private:
  std::list<CommandPtr> scheduled_commands;
  std::list<Subsystem *> registered_subsystems;

  void mainloop_tasks();
};
}  // namespace uvl