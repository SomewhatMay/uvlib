#pragma once

#include "main.h"
#include "uvlib/enums.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * One of the two joysticks on the default VEX controller.
 *
 * Use this to poll inputs from the joysticks.
 */
class Joystick {
 public:
  explicit Joystick(pros::Controller* controller, AnalogStick stick);

  /**
   * @return The current x position of this joystick.
   */
  int get_x() const;

  /**
   * @return The current y position of this joystick.
   */
  int get_y() const;

  /**
   * Return a readonly reference to controller that this
   * trigger is connected to.
   *
   * @return The connected controller.
   */
  const pros::Controller& get_controller() const { return *controller; }

 private:
  pros::Controller* controller;

  AnalogStick stick;
};

}  // namespace uvl