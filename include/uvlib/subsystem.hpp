#pragma once

#include <memory>

#include "uvlib/typedefs.hpp"

namespace uvlib {
class Subsystem {
 private:
  /**
   * Keeps track of whether the subsystem was used
   * by a command this current periodic tick.
   *
   * Although not recommended, this value can be safely
   * used in the subsystem's periodic() method. If a
   * default_command for the subsystem is present, this value
   * will *always* yield true in the periodic() method. Updating
   * this value in the periodic() method does nothing.
   */
  bool used_current_tick = false;

  /**
   * The command to be automatically executed if no commands are being executed
   * at the moment
   */
  std::shared_ptr<Command> default_command;

 protected:
  /**
   * Internal automatic method. Is called automatically in the
   * Subsystem's default constructor.
   * Will register the calling subsystem to the scheduler.
   */
  void register_self();

 public:
  /**
   * Automatically registers this subsystem to the
   * scheduler with the active register_self() method.
   */
  Subsystem();

  virtual void initialize();

  virtual void periodic();

  /* Getters and Setters */

  void set_default_command(std::shared_ptr<Command> command);

  /**
   * In-place construction and assignment of command.
   * The most preferred overload of method.
   */
  template <typename DerivedCommand, typename... Args>
  cmdptr<DerivedCommand> set_default_command(Args&&... constructor_args);

  std::shared_ptr<Command>& get_default_command();

  bool get_used_current_tick() const;

  void set_used_current_tick(bool used_current_tick);
};
}  // namespace uvlib