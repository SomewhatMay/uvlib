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
  /**
   * Determines whether the controller should be
   * destroyed when this object is destroyed.
   */
  bool controller_ownership = false;

  pros::Controller* controller;

  std::unordered_map<TriggerButton, Trigger> binded_triggers;

  Joystick m_left_joystick;
  Joystick m_right_joystick;

 public:
  explicit Controller(pros::Controller* controller);

  explicit Controller(pros::controller_id_e_t id);

  ~Controller();

  void periodic() override;

  Trigger& get_trigger(TriggerButton button);

  const Joystick& left_joystick() { return m_left_joystick; }

  const Joystick& right_joystick() { return m_right_joystick; }

  /**
   * To use any native pros::Controller methods, you may
   * utilize the Controller::get_controller() method.
   *
   * Ex:
   * uvl::Controller master(E_CONTROLLER_MASTER);
   * master.get_controller().controller_rumble(". - . -");
   */

  /** Getters and Setters */

  const pros::Controller& get_controller() const { return *controller; };

  const std::unordered_map<TriggerButton, Trigger>& get_binded_triggers()
      const {
    return binded_triggers;
  };
};
}  // namespace uvl