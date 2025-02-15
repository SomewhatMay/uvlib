#pragma once

#include "main.h"
#include "uvlib/typedefs.hpp"

#include "uvlib/commands/commandptr.hpp"

namespace uvl {
class Trigger {
 public:
  explicit Trigger(pros::Controller*, TriggerButton button);

  Trigger(const Trigger&) = delete;
  Trigger& operator=(const Trigger&) = delete;

  Trigger(Trigger&&) = delete;
  Trigger& operator=(Trigger&&) = delete;

  /**
   * Checks and executes all binded triggers.
   * Executed by the parent controller every tick.
   */
  void execute();

  /* Trigger Methods */

  Trigger& on_true(CommandPtr&& command);

  Trigger& on_false(CommandPtr&& command);

  Trigger& on_change(CommandPtr&& command);

  Trigger& while_true(CommandPtr&& command);

  Trigger& while_false(CommandPtr&& command);

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
};
}  // namespace uvl