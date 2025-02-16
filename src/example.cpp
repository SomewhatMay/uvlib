/**
 * A very simple robot program to show some of the awesome
 * features of uvlib.
 */

#include <functional>
#include <iostream>

#include "main.h"
#include "uvlib/command_api.hpp"
#include "uvlib/enums.hpp"
#include "uvlib/input_api.hpp"
#include "uvlib/scheduler.hpp"
#include "uvlib/subsystem.hpp"
#include "uvlib/typedefs.hpp"

/* Simple Example Drivetrain Subsystem */
/* In production code, each subsystem should be in its own file */
class Drivetrain : public uvl::Subsystem {
 public:
  Drivetrain() { std::cout << "Drivetrain constructed" << std::endl; }

  void initialize() override {
    std::cout << "Drivetrain initialized" << std::endl;
  }

  void periodic() override { std::cout << "Drivetrain periodic" << std::endl; }

  void set_voltage(int voltage) {
    std::cout << "Drivetrain voltage: " << voltage << std::endl;
  }

  int get_position() { return m_position; }

  void set_position(int pos) {
    std::cout << "Drivetrain position: " << pos << std::endl;
    m_position = pos;
  }

  int get_velocity() { return m_velocity; }

  void set_velocity(int vel) {
    std::cout << "Drivetrain velocity: " << vel << std::endl;
    m_velocity = vel;
  }

 private:
  int m_position = 0;
  int m_velocity = 0;
};

/* Simple Example Commands */
/* In production code, each command should be in its own file */

/**
 * @brief Move the drivetrain forward for a specific duration
 */
class MoveForwardDuration
    : public uvl::CommandHelper<uvl::Command, MoveForwardDuration> {
 public:
  /**
   * @param drivetrain Pointer to the DriveTrain subsystem
   * @param duration Duration in miliseconds to move forward for
   */
  MoveForwardDuration(Drivetrain* drivetrain, double duration)
      : m_drivetrain(drivetrain), start(pros::millis()), m_duration(duration) {
    add_requirements({drivetrain});
  }

  void initialize() override { m_drivetrain->set_voltage(127); }

  bool is_finished() override { return pros::millis() - start > m_duration; }

 private:
  Drivetrain* m_drivetrain;
  uint32_t start;
  double m_duration;
};

/**
 * @brief Follow the controller's joystick input and move the drivetrain
 * accordingly.
 */
class MoveFollowController
    : public uvl::CommandHelper<uvl::Command, MoveFollowController> {
 public:
  /**
   * @param drivetrain Pointer to the DriveTrain subsystem
   * @param controller Pointer to the controller
   */
  MoveFollowController(Drivetrain* drivetrain, uvl::Controller* controller)
      : m_drivetrain(drivetrain), m_left_joystick(controller->left_joystick()) {
    add_requirements({drivetrain});
  }

  void initialize() override {
    m_drivetrain->set_voltage(0);
    m_drivetrain->set_position(0);
  }

  void execute() override {
    m_drivetrain->set_voltage(m_left_joystick.get_y());
  }

  bool is_finished() override { return false; }

 private:
  Drivetrain* m_drivetrain;
  uvl::Joystick m_left_joystick;
};

/* Executed when the robot is enabled in Operator Control mode */
void example_opcontrol() {
  // Subsystems are passed around by reference. As a result,
  // it's usually just convenient to create them on the heap.
  // Remember, there should only be one instance of each subsystem.
  Drivetrain* drivetrain = new Drivetrain();

  // Just a simple wrapper around the pros::Controller to make binding commands
  // easier.
  uvl::Controller master(pros::E_CONTROLLER_MASTER);

  // Set the default command for the drivetrain. This will run when there are no
  // other commands running that use the drivetrain subsystem.
  drivetrain->set_default_command(
      MoveFollowController(drivetrain, &master).to_ptr());

  // Move forward when the user presses the A button.
  // Uses regular 'class' commands.
  master.get_trigger(uvl::TriggerButton::kA)
      .on_true(MoveForwardDuration(drivetrain, 1000).to_ptr());

  // Immediately stop the drivetrain when the L1 button is pressed.
  // Uses advanced 'functional' commands. This case makes use of the
  // "InstantCommand" command.
  master.get_trigger(uvl::TriggerButton::kL1)
      .on_true(uvl::InstantCommand([&]() { drivetrain->set_voltage(0); },
                                   {drivetrain})
                   .to_ptr());

  // Initialize the scheduler. This will initialize all subsystems.
  uvl::Scheduler::get_instance().initialize();

  // Start the scheduler event loop, running all commands and subsystems every
  // 20ms.
  uvl::Scheduler::get_instance().mainloop();
}