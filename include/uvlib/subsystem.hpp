#pragma once

#include <memory>
#include <optional>

#include "uvlib/commands/commandptr.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * Contain any abstraction between the hardware and the software.
 *
 * Ex. Drivetrain, arm, elevator, etc.
 */
class Subsystem {
  friend class Scheduler;
  friend bool is_runnable_command(Command* command);

 public:
  /**
   * Automatically registers this subsystem to the scheduler with the active
   * register_self() method.
   */
  Subsystem();

  const std::optional<CommandPtr>& get_default_command() const;

  /**
   * An optional default command can be set for each subsystem that is executed
   * if no other command utilizes the subsystem for any given tick.
   *
   * The default subsystem is guaranteed to be ended every time another command
   * that depends on this subsystem executes.
   *
   * It is also guaranteed to be reinitialized every time this subsystem is free
   * and the default command was dead in the previous tick.
   *
   * @note The subsystem becomes responsible for the memory management of the
   * command.
   *
   * @param command The default command.
   */
  void set_default_command(CommandPtr&& command);

 protected:
  virtual ~Subsystem() = default;

  /**
   * Put any initialization code for the subsystem. This only runs once,
   * synchronously with other subsystems.
   */
  virtual void initialize();

  /**
   * Called periodically by the Scheduler after all commands have executed.
   */
  virtual void periodic();

 private:
  /**
   * The command to be automatically executed if no commands are being executed
   * at the moment
   */
  std::optional<CommandPtr> m_default_command;

  /**
   * Internal automatic method. Is called automatically in the Subsystem's
   * default constructor. This will register the calling subsystem to the
   * scheduler.
   */
  void register_self();
};
}  // namespace uvl