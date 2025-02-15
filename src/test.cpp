#include <functional>
#include <iostream>

#include "uvlib/command_api.hpp"
#include "uvlib/enums.hpp"
#include "uvlib/input_api.hpp"
#include "uvlib/scheduler.hpp"
#include "uvlib/subsystem.hpp"
#include "uvlib/typedefs.hpp"

class Motor : public uvl::Subsystem {
 public:
  void set_voltage(int voltage) {
    std::cout << "voltage updated to " << voltage << std::endl;
  }
};

class MoveMotor : public uvl::CommandHelper<uvl::Command, MoveMotor> {
 public:
  MoveMotor(Motor* motor) : motor(motor) { add_requirements({motor}); }

  MoveMotor(Motor* motor, std::function<int()> direction)
      : motor(motor), direction(direction) {
    add_requirements({motor});
  }

  void execute() override { motor->set_voltage(12); }

 private:
  std::function<int()> direction;
  Motor* motor;
};

int test() {
  Motor* motor = new Motor();
  motor->set_default_command(std::move(MoveMotor(motor).to_ptr()));

  uvl::Controller master(pros::E_CONTROLLER_MASTER);

  master.get_trigger(uvl::TriggerButton::kA)
      .on_true(MoveMotor(motor, [&master]() {
                 return master.left_joystick().get_x();
               }).to_ptr());

  uvl::Scheduler::get_instance().register_subsystem(motor);

  uvl::Scheduler::get_instance().schedule_command(MoveMotor(motor).to_ptr());
  uvl::Scheduler::get_instance().schedule_command(
      std::move(MoveMotor(motor).to_ptr()));

  uvl::Scheduler::get_instance().cancel_command(MoveMotor(motor).to_ptr());
  uvl::Scheduler::get_instance().cancel_command(
      std::move(MoveMotor(motor).to_ptr()));

  return 0;
}