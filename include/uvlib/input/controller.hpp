#pragma once

#include "main.h"
#include "uvlib/enums.hpp"
#include "uvlib/input/joystick.hpp"
#include "uvlib/input/trigger.hpp"
#include "uvlib/subsystem.hpp"

namespace uvl {
class Scheduler;
/**
 * Manages input variants from a specific controller.
 *
 * Provides interfaces to use the Trigger object.
 *
 * @note Controllers cannot be copied or moved around. They must be created once
 * and exist for the entire lifetime of the robot to function properly.
 */
class Controller : public Subsystem {
 public:
  explicit Controller(pros::Controller* controller);

  explicit Controller(pros::controller_id_e_t id);

  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

  Controller(Controller&&) = delete;
  Controller& operator=(Controller&&) = delete;

  ~Controller();

  /**
   * Get a non-copyable, reference to a specific button on the VEX controller
   * that can be used to bind commands.
   *
   * @param button A specific digital button on the VEX controller to target.
   *
   * @return The controller's trigger.
   */
  Trigger& get_trigger(TriggerButton button);

  /**
   * A readonly reference to the left joystick of the VEX controller. Can be
   * used to poll for joystick input.
   *
   * @return The controller's left joystick.
   */
  const Joystick& left_joystick() { return m_left_joystick; }

  /**
   * A readonly reference to the right joystick of the VEX controller. Can be
   * used to poll for joystick input.
   *
   * @return The controller's right joystick.
   */
  const Joystick& right_joystick() { return m_right_joystick; }

  /** Getters and Setters */

  /**
   * To use any native pros::Controller methods, you may utilize retrieve it
   * with this method.
   *
   * Ex:
   ```cpp
   uvl::Controller master(E_CONTROLLER_MASTER);
   master.get_controller().controller_rumble(". - . -");
   ```
   */
  const pros::Controller& get_controller() const { return *controller; };

  /**
   * Return a list of all triggers the controller has created.
   *
   * @note Some of the triggers may not be binded currently.
   *
   * @return A reference to all the binded triggers.
   */
  const std::unordered_map<TriggerButton, Trigger>& get_binded_triggers()
      const {
    return binded_triggers;
  };

 private:
  friend class Scheduler;

  /**
   * Determines whether the controller should be destroyed when this object is
   * destroyed.
   */
  bool controller_ownership = false;

  pros::Controller* controller;

  std::unordered_map<TriggerButton, Trigger> binded_triggers;

  Joystick m_left_joystick;
  Joystick m_right_joystick;

  void periodic() override;
};
}  // namespace uvl