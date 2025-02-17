#include "uvlib/input/controller.hpp"

#include "main.h"
#include "uvlib/input/joystick.hpp"
#include "uvlib/input/trigger.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {

Controller::Controller(pros::Controller* controller)
    : controller(controller),
      m_left_joystick(controller, AnalogStick::kLeft),
      m_right_joystick(controller, AnalogStick::kRight) {}

Controller::Controller(pros::controller_id_e_t id)
    : controller(new pros::Controller(id)),
      m_left_joystick(controller, AnalogStick::kLeft),
      m_right_joystick(controller, AnalogStick::kRight) {
  controller_ownership = true;
}

Controller::~Controller() {
  unbind_all_triggers();

  if (controller_ownership) {
    delete controller;
  }
}

Trigger& Controller::get_trigger(TriggerButton button) {
  auto trigger = binded_triggers.try_emplace(button, controller, button);

  return trigger.first->second;
}

void Controller::unbind_all_triggers() {
  // Unbinds all triggers and calls their destructors.
  binded_triggers.clear();
}

void Controller::periodic() {
  for (auto& [button, trigger] : binded_triggers) {
    trigger.execute();
  }
}
}  // namespace uvl