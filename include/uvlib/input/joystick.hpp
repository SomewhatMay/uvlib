#pragma once

#include "main.h"
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
};

}  // namespace uvl