#pragma once

#include "main.h"
#include "uvlib/commands/commandptr.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * A wrapper to the digital buttons on the default VEX controller.
 *
 * Use this to directly bind Commands to controller behaviour/inputs.
 */
class Trigger {
public:
  explicit Trigger(pros::Controller *, TriggerButton button);

  /**
   * All commands binded to this trigger are automatically unbinded and
   * cancelled upon destruction.
   */
  ~Trigger();

  /**
   * @note Triggers can not be moved or copied. You must retrieve the trigger
   * from the appropriate Controller using Controller::get_trigger();
   */
  Trigger(const Trigger &) = delete;
  Trigger &operator=(const Trigger &) = delete;

  Trigger(Trigger &&) = delete;
  Trigger &operator=(Trigger &&) = delete;

  /**
   * @brief Return true if and only if the trigger is currently being held down.
   *
   * Analogous to the pros::Controller::get_digital(<TriggerButton>);
   *
   * @return true
   * @return false
   */
  bool poll();

  /* Trigger Methods */

  /**
   * Executed when the specified trigger's status turns from false to true.
   *
   * Ex. when the user presses the right trigger, schedule command to be
   * executed. Do nothing if the user continues to hold right trigger.
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &on_true(CommandPtr &&command);

  /**
   * Unbind the on_true trigger. Any related command that is scheduled by this
   * specific trigger method will be cancelled.
   */
  Trigger &on_true(std::nullopt_t);

  /**
   * Same as Trigger::on_true, except schedules the command when the trigger's
   * state goes from true to false.
   *
   * @see Trigger:on_true();
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &on_false(CommandPtr &&command);

  /**
   * Unbind the on_false trigger. Any related command that is scheduled by this
   * specific trigger method will be cancelled.
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &on_false(std::nullopt_t);

  /**
   * Executed when the specified trigger's status turns from false to true or
   * vice versa.
   *
   * Ex. when the user presses the right trigger, schedule command to be
   * executed. Do nothing if the user continues to hold right trigger.
   * Reschedule command if the user lets go of the right trigger.
   *
   * @see Trigger::on_true();
   * @see Trigger::on_false();
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &on_change(CommandPtr &&command);

  /**
   * Unbind the on_change trigger. Any related command that is scheduled by this
   * specific trigger method will be cancelled.
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &on_change(std::nullopt_t);

  /**
   * Schedule command when the user initially presses the trigger. When the
   * trigger is released, cancel the command if it has not successfully
   * completed yet.
   *
   * Internally, this method calls Command::cancel() when the trigger
   * is released.
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &while_true(CommandPtr &&command);

  /**
   * Unbind the while_true trigger. Any related command that is scheduled by
   * this specific trigger method will be cancelled.
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &while_true(std::nullopt_t);

  /**
   * Opposite behaviour to while_true()
   *
   * @see `Trigger::while_true()`
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &while_false(CommandPtr &&command);

  /**
   * Unbind the while_false trigger. Any related command that is scheduled by
   * this specific trigger method will be cancelled.
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &while_false(std::nullopt_t);

  /**
   * Unbinds all binded actions. Any related command that is scheduled by this
   * specific trigger method will be cancelled.
   *
   * @returns The trigger that was called on. Enables method chaining.
   */
  Trigger &unbind_all();

  /**
   * Return a readonly reference to controller that this
   * trigger is connected to.
   */
  const pros::Controller &get_controller() const { return *controller; }

private:
  friend class Controller;

  pros::Controller *controller;

  TriggerButton button;

  bool previous_state = false;

  std::optional<CommandPtr> m_on_true;
  std::optional<CommandPtr> m_on_false;
  std::optional<CommandPtr> m_on_change;
  std::optional<CommandPtr> m_while_true;
  std::optional<CommandPtr> m_while_false;

  /**
   * Checks and executes all binded triggers. Executed by the parent controller
   * every tick.
   */
  void execute();
};
} // namespace uvl