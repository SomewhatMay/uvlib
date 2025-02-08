#pragma once

#include <bitset>
#include <list>
#include <memory>
#include <stack>

namespace uvlib {
class Subsystem;
class Command;

using command_chain_t = std::list<std::shared_ptr<Command>>;
using command_list_t = std::list<command_chain_t>;
using subsystem_list_t = std::list<Subsystem *>;

/**
 * NOTE: This class should not be manually instantiated. Instead,
 * use the get_scheduler() method as defined below.
 */
class Scheduler {
 private:
  command_list_t scheduled_commands;
  subsystem_list_t registered_subsystems;

  void mainloop_tasks();

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
  const subsystem_list_t::iterator &register_subsystem(Subsystem *subsystem);

  /**
   * Internal method only: to be used by the command superclass
   * automatically.
   */
  const command_list_t::iterator &schedule_command(
      std::shared_ptr<Command> command);

  void cancel_command(command_chain_t::iterator command_iterator);

  const command_list_t &get_scheduled_commands() const;

  const subsystem_list_t &get_subsystems();
};

/**
 * Returns the active scheduler. Creates a new
 * scheduler the first time it is called.
 *
 * NOTE: Always use this method instead of instantiating
 * a new Scheduler object.
 */
Scheduler &get_scheduler();
}  // namespace uvlib