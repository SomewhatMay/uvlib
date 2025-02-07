#pragma once

#include <vector>

#include "uvlib/scheduler.hpp"
#include "uvlib/subsystem.hpp"

namespace uvlib {
class Command {
 private:
  command_chain_t::iterator command_iterator;
  std::list<Subsystem*> requirements;

 protected:
  void add_requirements(const std::vector<Subsystem>& requirements);

 public:
  Command();

  void set_command_iterator(const command_chain_t::iterator& command_iterator);

  const command_chain_t::iterator& get_command_iterator() const;

  void cancel();

  const std::list<Subsystem*>& get_requirements() const;

  virtual void initialize();

  virtual void execute();

  virtual bool is_finished() = 0;

  virtual void end(bool interrupted);
};

}  // namespace uvlib