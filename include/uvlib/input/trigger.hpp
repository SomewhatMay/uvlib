#pragma once

#include "main.h"
#include "uvlib/commands/commandptr.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * A wrapper to the digital buttons on the default
 * VEX controller.
 *
 * Use this to directly bind Commands to controller
 * behaviour/inputs.
 */
class Trigger {
 public:
  explicit Trigger(pros::Controller*, TriggerButton button);

  /**
   * NOTE: Triggers can not be moved or copied.
   *        You must retrieve the trigger from the
   *        appropriate Controller using
   *        Controller::get_trigger();
   */
  Trigger(const Trigger&) = delete;
  Trigger& operator=(const Trigger&) = delete;

  Trigger(Trigger&&) = delete;
  Trigger& operator=(Trigger&&) = delete;

  /* Trigger Methods */

  /**
   * Executed when the specified trigger's status turns from
   * false to true.
   *
   * Ex. when the user presses the right trigger, schedule
   * command to be executed. Do nothing if the user continues
   * to hold right trigger.
   */
  Trigger& on_true(CommandPtr&& command);

  /**
   * Same as Trigger::on_true, except schedules the command
   * when the trigger's state goes from true to false.
   *
   * @see Trigger:on_true();
   */
  Trigger& on_false(CommandPtr&& command);

  /**
   * Executed when the specified trigger's status turns from
   * false to true or vice versa.
   *
   * Ex. when the user presses the right trigger, schedule
   * command to be executed. Do nothing if the user continues
   * to hold right trigger. Reschedule command if the user lets
   * go of the right trigger.
   *
   * @see Trigger:on_true();
   * @see Trigger:on_false();
   */
  Trigger& on_change(CommandPtr&& command);

  /**
   * Schedule command when the user initially presses the trigger.
   * When the trigger is released, cancel the command if it has not
   * successfully completed yet.
   *
   * Internally, this method calls Command::cancel() when the trigger
   * is released.
   */
  Trigger& while_true(CommandPtr&& command);

  /**
   * Opposite behaviour to while_true()
   *
   * @see Trigger::while_true()
   */
  Trigger& while_false(CommandPtr&& command);

  /**
   * Return a readonly reference to controller that this
   * trigger is connected to.
   */
  const pros::Controller& get_controller() const { return *controller; }

 private:
  pros::Controller* controller;

  TriggerButton button;

  bool previous_state = false;

  std::optional<CommandPtr> m_on_true;
  std::optional<CommandPtr> m_on_false;
  std::optional<CommandPtr> m_on_change;
  std::optional<CommandPtr> m_while_true;
  std::optional<CommandPtr> m_while_false;

  /**
   * Checks and executes all binded triggers.
   * Executed by the parent controller every tick.
   */
  void execute();
};
}  // namespace uvl