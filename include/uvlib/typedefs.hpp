#pragma once

#include <forward_list>
#include <list>
#include <memory>

#include "main.h"

namespace uvl {

class Subsystem;
class Command;
class Trigger;
class Joystick;

/**
 * Determines the order that a command is added
 * to the list of commands that will be executed
 * by the scheduler. The scheduler executes commands
 * from top to bottom; therefore, by default, adding
 * commands to the list behaves like a stack.
 */
enum struct ScheduleDirection {
  kTop = 0,  // DEFAULT
  kBottom = 1
};

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

enum struct AnalogStick { kLeft = 0, kRight = 1 };

using commandptr_t = std::shared_ptr<Command>;

template <typename DerivedCommand>
using cmdptr = std::enable_if_t<std::is_base_of_v<Command, DerivedCommand>,
                                std::shared_ptr<DerivedCommand>>;

using command_list_t = std::list<commandptr_t>;
using subsystem_list_t = std::list<Subsystem *>;

}  // namespace uvl