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

 protected:
  void set_requirements(const std::initializer_list<Subsystem>& requirements);

 public:
  Command();

  void set_command_iterator(const command_chain_t::iterator& command_iterator);

  const command_chain_t::iterator& get_command_iterator() const;

  void cancel();

  std::list<Subsystem*>& get_requirements() const;

  void set_tick_number(int tick_number);

  int get_tick_number() const;

  virtual void initialize();

  virtual void execute();

  virtual bool is_finished() = 0;

  virtual void end(bool interrupted);
};

}  // namespace uvlib