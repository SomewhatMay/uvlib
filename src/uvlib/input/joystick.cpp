#include "uvlib/input/joystick.hpp"

#include "main.h"
#include "uvlib/typedefs.hpp"

namespace uvl {
Joystick::Joystick(pros::Controller* controller, AnalogStick stick)
    : controller(controller), stick(stick) {}

int Joystick::get_x() const {
  pros::controller_analog_e_t analog_e;

  if (stick == AnalogStick::kLeft)
    analog_e = pros::controller_analog_e_t::E_CONTROLLER_ANALOG_LEFT_X;
  else
    analog_e = pros::controller_analog_e_t::E_CONTROLLER_ANALOG_RIGHT_X;

  return controller->get_analog(analog_e);
}

int Joystick::get_y() const {
  pros::controller_analog_e_t analog_e;

  if (stick == AnalogStick::kLeft)
    analog_e = pros::controller_analog_e_t::E_CONTROLLER_ANALOG_LEFT_Y;
  else
    analog_e = pros::controller_analog_e_t::E_CONTROLLER_ANALOG_RIGHT_Y;

  return controller->get_analog(analog_e);
}
}  // namespace uvl
