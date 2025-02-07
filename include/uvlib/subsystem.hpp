#pragma once

#include "uvlib/command.hpp"

namespace uvlib {
class Subsystem {
 private:
  int subsystem_id;  // [0, 64] --> Restricted by the bitset size in
                     // scheduler.hpp

 protected:
  /**
   * Internal automatic method. Is called automatically in the
   * Subsystem's default constructor.
   * Will register the calling subsystem to the scheduler.
   *
   * Ensure a Scheduler has been initialized before this
   * function is called.
   *
   * Sets subsystem_id to a valid value.
   */
  void register_self();

 public:
  Subsystem();

  void set_default_command(Command command);

  void run(Command command);

  virtual void initialize();

  virtual void periodic();
};
}  // namespace uvlib