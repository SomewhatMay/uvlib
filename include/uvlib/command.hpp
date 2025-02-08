#pragma once

#include <vector>

#include "uvlib/scheduler.hpp"

namespace uvlib {
class Subsystem;

class Command {
 private:
  command_chain_t::iterator command_iterator;
  std::list<Subsystem*> requirements;

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
  bool alive = false;

 protected:
  void set_requirements(const std::initializer_list<Subsystem*>& requirements);

 public:
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
   * Cancels the current command.
   *
   * WARNING: This command should NEVER be called within
   * it's execute() method. Ex. If you have a command
   * called Drive, you should NEVER call Drive.cancel()
   * in Drive's execute() function.
   */
  void cancel();

  /* Getters and Setters */

  /**
   * Internal method.
   */
  void set_command_iterator(const command_chain_t::iterator& command_iterator);

  /**
   * Internal method.
   */
  const command_chain_t::iterator& get_command_iterator() const;

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