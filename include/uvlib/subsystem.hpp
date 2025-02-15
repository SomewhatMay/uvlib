#pragma once

#include <memory>
#include <optional>

#include "uvlib/commands/commandptr.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
class Subsystem {
  friend class Scheduler;
  friend bool is_runnable_command(Command* command);

 public:
  /**
   * Automatically registers this subsystem to the
   * scheduler with the active register_self() method.
   */
  Subsystem();

  const std::optional<CommandPtr>& get_default_command() const;

  /**
   * An optional default command can be set for
   * each subsystem that is executed if no other
   * command utilizes the subsystem for any given
   * tick.
   *
   * The default subsystem is guaranteed to be
   * ended every time another command that depends
   * on this subsystem executes.
   *
   * It is also guaranteed to be reinitialized every
   * time this subsystem is free and the default command
   * was dead in the previous tick.
   */
  void set_default_command(CommandPtr&& command);

 protected:
  virtual ~Subsystem() = default;

  /**
   * Put any initialization code for the subsystem.
   * This only runs once, synchronously with other
   * subsystems.
   */
  virtual void initialize();

  /**
   * Called periodically by the Scheduler after
   * all commands have executed.
   */
  virtual void periodic();

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
  bool m_used_current_tick = false;

  /**
   * The command to be automatically executed if no commands are being executed
   * at the moment
   */
  std::optional<CommandPtr> m_default_command;

  /**
   * Internal automatic method. Is called automatically in the
   * Subsystem's default constructor.
   * Will register the calling subsystem to the scheduler.
   */
  void register_self();
};
}  // namespace uvl