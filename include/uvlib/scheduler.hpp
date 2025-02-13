#pragma once

#include <bitset>
#include <list>
#include <memory>
#include <stack>

#include "uvlib/singleton.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {

/**
 * WARNING: This class should not be manually instantiated. Instead,
 * use the Scheduler::get_instance() method instead.
 */
class Scheduler : public Singleton<Scheduler> {
 private:
  command_list_t scheduled_commands;
  subsystem_list_t registered_subsystems;

  void mainloop_tasks();

 public:
  /**
   * Use
   *
   * static T& Scheduler::get_instance();
   *
   * to get the active scheduler. Creates a new
   * scheduler the first time it is called.
   *
   * NOTE: Always use this method instead of instantiating
   * a new Scheduler object.
   */

  void initialize();

  /**
   * Does not run on a separate task. If mainloop is
   * called in a separate task, ensure that all accessed
   * variables are atomic.
   */
  void mainloop();

  /**
   * Internal method only: to be used by the subsystem superclass
   * automatically.
   */
  void register_subsystem(Subsystem *subsystem);

  template <typename DerivedCommand>
  cmdptr<DerivedCommand> schedule_command(cmdptr<DerivedCommand> command);

  template <typename DerivedCommand, typename... Args>
  cmdptr<DerivedCommand> schedule_command(Args &&...constructor_args);

  /**
   * Sets the command as not alive so it is not executed
   * in future ticks (unless rescheduled) and is also removed
   * from the command_list_t in the next tick.
   */
  void cancel_command(std::shared_ptr<Command> command);

  const command_list_t &get_scheduled_commands() const;

  const subsystem_list_t &get_subsystems();
};
}  // namespace uvl