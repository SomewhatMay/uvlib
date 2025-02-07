#pragma once

#include <list>
#include <stack>

#include "uvlib/command.hpp"
#include "uvlib/subsystem.hpp"

namespace uvlib {
using command_list_t = std::list<std::list<Command>>;
using subsystem_list_t = std::list<Subsystem>;

/**
 * NOTE: This class should not be manually instantiated. Instead,
 * use the get_scheduler() method as defined below.
 */
class Scheduler {
 private:
  command_list_t scheduled_commands;
  subsystem_list_t registered_subsystems;

 public:
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
  void register_subsystem(Subsystem subsystem);

  /**
   * Internal method only: to be used by the command superclass
   * automatically.
   */
  void schedule_command(Command command);

  void schedule_command(Command parent_command, Command child_command);

  const command_list_t &get_scheduled_commands();

  const subsystem_list_t &get_subsystems();
};

/**
 * Always use this method instead of instantiating
 * a new Scheduler object.
 */
const Scheduler &get_scheduler();
}  // namespace uvlib