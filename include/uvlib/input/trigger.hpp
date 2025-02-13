#pragma once

#include "main.h"
#include "typedefs.hpp"

namespace uvl {
class Trigger {
 private:
  const pros::Controller* controller;

 public:
  Trigger(const pros::Controller& controller) {
    this->controller = &controller;
  }

  /* Trigger Methods */

  /* trigger: on_true */
  Trigger on_true(cmdptr<Command> command);

  template <typename DerivedCommand, typename... Args>
  Trigger on_true(Args&&... constructor_args);

  /* Getters and Setters */

  const pros::Controller& get_controller() const;
};
}  // namespace uvl