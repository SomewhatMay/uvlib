#pragma once

#include "main.h"
#include "uvlib/enums.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
class Joystick {
 private:
  pros::Controller* controller;

  AnalogStick stick;

 public:
  explicit Joystick(pros::Controller* controller, AnalogStick stick);

  int get_x() const;

  int get_y() const;

  const pros::Controller& get_controller() const { return *controller; }
};

}  // namespace uvl