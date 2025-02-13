#pragma once

#include "main.h"
#include "uvlib/subsystem.hpp"

namespace uvl {
/**
 * Manages input variants from a specific controller.
 *
 * Provides interfaces to use the Trigger object.
 */
class Controller : public Subsystem {
 private:
  bool own_controller = false;
  pros::Controller* controller;

 public:
  Controller(pros::Controller& controller) {
    this->controller = &controller;
  }

  ~Controller() {
  }

  const pros::Controller& get_controller() const;
};
}  // namespace uvl