#include "main.h"
#include "uvlib/command_api.hpp"
#include "uvlib/input_api.hpp"
#include "uvlib/typedefs.hpp"

class MyCommand : public uvl::Command {
 public:
  MyCommand() = default;

  MyCommand(std::string greeting) { std::cout << greeting << '\n'; }

  bool is_finished() { return false; }
};

int test() {
  uvl::cmdptr<MyCommand> myCommand(new MyCommand());
  uvl::cmdptr<MyCommand> secondCommand(new MyCommand());

  myCommand->and_then(secondCommand)->and_then<MyCommand>();

  uvl::Controller controller(pros::E_CONTROLLER_MASTER);
  controller.get_trigger(uvl::TriggerButton::kA).on_true(myCommand);
  controller.left_joystick().get_x();

  return 0;
}
