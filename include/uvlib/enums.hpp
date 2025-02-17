#pragma once

#include "main.h"

namespace uvl {
/**
 * Determines the order that a command is added to the list of commands that
 * will be executed by the scheduler. The scheduler executes commands from top
 * to bottom; therefore, by default, adding commands to the list behaves like a
 * stack.
 */
enum struct ScheduleDirection {
  kTop,  // Default
  kBottom
};

/**
 * Possible input trigger buttons; analogous to pros digital buttons.
 */
enum struct TriggerButton {
  kL1 = pros::E_CONTROLLER_DIGITAL_L1,
  kL2 = pros::E_CONTROLLER_DIGITAL_L2,
  kR1 = pros::E_CONTROLLER_DIGITAL_R1,
  kR2 = pros::E_CONTROLLER_DIGITAL_R2,
  kUp = pros::E_CONTROLLER_DIGITAL_UP,
  kDown = pros::E_CONTROLLER_DIGITAL_DOWN,
  kLeft = pros::E_CONTROLLER_DIGITAL_LEFT,
  kRight = pros::E_CONTROLLER_DIGITAL_RIGHT,
  kX = pros::E_CONTROLLER_DIGITAL_X,
  kB = pros::E_CONTROLLER_DIGITAL_B,
  kY = pros::E_CONTROLLER_DIGITAL_Y,
  kA = pros::E_CONTROLLER_DIGITAL_A
};

/**
 * The two analog sticks in a standard VEX controller.
 */
enum struct AnalogStick { kLeft, kRight };
}  // namespace uvl