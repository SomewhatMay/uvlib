#pragma once

#include "main.h"
#include "uvlib/typedefs.hpp"

namespace uvl {
class Trigger {
 private:
  pros::Controller* controller;

  TriggerButton button;

  bool previous_state = false;

  std::optional<commandptr_t> m_on_true;
  std::optional<commandptr_t> m_on_false;
  std::optional<commandptr_t> m_on_change;
  std::optional<commandptr_t> m_while_true;
  std::optional<commandptr_t> m_while_false;

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
  Trigger& on_true(Args&&... constructor_args) {
    return on_true(std::make_shared<DerivedCommand>(
        std::forward<Args>(constructor_args)...));
  }

  /* trigger: on_false */
  Trigger& on_false(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  Trigger& on_false(Args&&... constructor_args) {
    return on_false(std::make_shared<DerivedCommand>(
        std::forward<Args>(constructor_args)...));
  }

  /* trigger: on_change */
  Trigger& on_change(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  Trigger& on_change(Args&&... constructor_args) {
    return on_change(std::make_shared<DerivedCommand>(
        std::forward<Args>(constructor_args)...));
  }

  /* trigger: while_true */
  Trigger& while_true(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  Trigger& while_true(Args&&... constructor_args) {
    return while_true(std::make_shared<DerivedCommand>(
        std::forward<Args>(constructor_args)...));
  }

  /* trigger: while_false */
  Trigger& while_false(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  Trigger& while_false(Args&&... constructor_args) {
    return while_false(std::make_shared<DerivedCommand>(
        std::forward<Args>(constructor_args)...));
  }

  /* Getters and Setters */

  const pros::Controller& get_controller() const { return *controller; }
};
}  // namespace uvl