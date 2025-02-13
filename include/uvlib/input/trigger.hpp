#pragma once

#include "main.h"
#include "typedefs.hpp"

namespace uvl {
class Trigger {
 private:
  pros::Controller* controller;

  TriggerButton button;

  bool previous_state = false;

  std::optional<commandptr_t> m_on_true;

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

  /* trigger: on_true */
  Trigger& on_true(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  Trigger& on_true(Args&&... constructor_args);

  /* Getters and Setters */

  const pros::Controller& get_controller() const { return *controller; }
};
}  // namespace uvl