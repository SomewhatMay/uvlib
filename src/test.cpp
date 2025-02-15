#include <iostream>

#include "uvlib/command_api.hpp"
#include "uvlib/enums.hpp"
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

  void execute() override { motor->set_voltage(12); }

 private:
  Motor* motor;
};

int test() {
  Motor* motor = new Motor();
  uvl::Scheduler::get_instance().register_subsystem(motor);

  uvl::Scheduler::get_instance().schedule_command(MoveMotor(motor).to_ptr());

  return 0;
}