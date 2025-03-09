#include "main.h"
#include "pros/misc.h"
#include "uvlib/commands/advanced_commands/function_command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/enums.hpp"
#include "uvlib/input/controller.hpp"
#include "uvlib/input/trigger.hpp"
#include "uvlib/scheduler.hpp"
#include <cstdint>

class Drivetrain : public uvl::Subsystem {
public:
  Drivetrain() : m_voltage(0), m_left_motor(0), m_right_motor(1) {}

  void set_voltage(int voltage) {
    m_voltage = voltage;
    m_left_motor.move(voltage);
    m_right_motor.move(-voltage);
  }

  int get_voltage() const { return m_voltage; }

private:
  pros::Motor m_left_motor;
  pros::Motor m_right_motor;

  int m_voltage;
};

class TeleopDrive : public uvl::CommandHelper<uvl::Command, TeleopDrive> {
public:
  TeleopDrive(Drivetrain *drivetrain, uvl::Controller *controller)
      : m_left_joystick(controller->left_joystick()), m_drivetrain(drivetrain) {
    add_requirements({drivetrain});
  }

  void initialize() override { m_drivetrain->set_voltage(0); }

  void execute() override {
    int input_y = m_left_joystick.get_y();
    m_drivetrain->set_voltage(input_y);
  }

  bool is_finished() override { return false; }

  void end(bool interrupted) override { m_drivetrain->set_voltage(0); }

private:
  Drivetrain *m_drivetrain;
  uvl::Joystick m_left_joystick;
};

Drivetrain drivetrain;
uvl::Controller master(pros::E_CONTROLLER_MASTER);

std::string routine_a_state = "uninitialized";
std::string routine_b_state = "uninitialized";
std::string routine_c_state = "uninitialized";

void noop() {}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();

  drivetrain.set_default_command(TeleopDrive(&drivetrain, &master).to_ptr());

  int artificial_tick = 0;

  /* Debugging Command */
  uvl::Scheduler::get_instance().schedule_command(
      uvl::FunctionCommand(
          []() {},
          [&artificial_tick]() {
            pros::lcd::set_text(0, "Tick Number: " +
                                       std::to_string(artificial_tick));
            pros::lcd::set_text(
                1, "Scheduled Commands: " +
                       std::to_string(uvl::Scheduler::get_instance()
                                          .get_scheduled_commands()
                                          .size()));
            pros::lcd::set_text(
                2, "Scheduler Owned: " +
                       std::to_string(uvl::Scheduler::get_instance()
                                          .get_owned_commands()
                                          .size()));

            pros::lcd::set_text(
                3, "Active Subsystems: " +
                       std::to_string(uvl::Scheduler::get_instance()
                                          .get_active_subsystems()
                                          .size()));

            pros::lcd::set_text(
                4, std::string("Drivetrain Default: ") +
                       (drivetrain.get_default_command()->get()->is_alive()
                            ? "alive"
                            : "paused"));

            pros::lcd::set_text(5,
                                std::string("Routine A: ") + routine_a_state);

            pros::lcd::set_text(6,
                                std::string("Routine B: ") + routine_b_state);

            pros::lcd::set_text(7,
                                std::string("Routine C: ") + routine_c_state);

            artificial_tick++;
          },
          []() { return false; }, [](bool) {}, {})
          .to_ptr());

  /* Initialize the scheduler */
  uvl::Scheduler::get_instance().initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  uint32_t routine_a_start = 0;
  uvl::FunctionCommand routine_a(
      [&routine_a_start]() {
        routine_a_state = "initialized";
        routine_a_start = pros::millis();
      },
      []() {
        routine_a_state =
            "executing - " + std::to_string(pros::millis()) + " " +
            std::to_string(master.get_digital(uvl::TriggerButton::kA));
      },
      [&routine_a_start]() { return pros::millis() >= routine_a_start + 4; },
      [](bool interrupted) {
        routine_a_state = interrupted ? "interrupted" : "success";
      },
      {&drivetrain});

  uint32_t routine_b_start = 0;
  uvl::FunctionCommand routine_b(
      [&routine_b_start]() {
        routine_b_state = "initialized";
        routine_b_start = pros::millis();
      },
      []() {
        routine_b_state =
            "executing - " + std::to_string(pros::millis()) + " " +
            std::to_string(master.get_digital(uvl::TriggerButton::kB));
      },
      [&routine_b_start]() { return pros::millis() >= routine_b_start + 10; },
      [](bool interrupted) {
        routine_b_state = interrupted ? "interrupted" : "success";
      },
      {&drivetrain});

  uint32_t routine_c_start_a = 0;
  uint32_t routine_c_start_b = 0;

  uvl::CommandPtr routine_c =
      uvl::FunctionCommand(
          [&routine_c_start_a]() {
            routine_c_start_a = pros::millis();
            routine_c_state = "A - initialized";
            drivetrain.set_voltage(50);
          },
          []() {
            routine_c_state =
                "A - executing - " + std::to_string(pros::millis());
          },
          [&routine_c_start_a]() {
            return pros::millis() >= routine_c_start_a + 1;
          },
          [](bool interrupted) {
            drivetrain.set_voltage(0);
            routine_c_state = interrupted ? "A - interrupted" : "A - success";
          },
          {&drivetrain})
          .and_then(uvl::FunctionCommand(
                        [&routine_c_start_b]() {
                          routine_c_start_b = pros::millis();
                          routine_c_state = "B - initialized";
                          drivetrain.set_voltage(-50);
                        },
                        []() {
                          routine_c_state = "B - executing - " +
                                            std::to_string(pros::millis());
                        },
                        [&routine_c_start_b]() {
                          return pros::millis() >= routine_c_start_b + 1;
                        },
                        [](bool interrupted) {
                          drivetrain.set_voltage(0);
                          routine_c_state =
                              interrupted ? "B - interrupted" : "B - success";
                        },
                        {&drivetrain})
                        .to_ptr());

  master.get_trigger(uvl::TriggerButton::kA)
      .on_true(std::move(routine_a).to_ptr());

  master.get_trigger(uvl::TriggerButton::kB)
      .while_true(std::move(routine_b).to_ptr());

  master.get_trigger(uvl::TriggerButton::kX).on_true(std::move(routine_c));

  uvl::Scheduler::get_instance().mainloop();
}