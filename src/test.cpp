#include "main.h"
#include "uvlib/command_api.hpp"
#include "uvlib/input_api.hpp"
#include "uvlib/scheduler.hpp"
#include "uvlib/subsystem.hpp"
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

  uvl::Controller master(pros::E_CONTROLLER_MASTER);
  uvl::Trigger& L1 = master.get_trigger(uvl::TriggerButton::kL1);
  L1.on_true<MyCommand>();

  myCommand->and_then(secondCommand)->and_then<MyCommand>();

  uvl::cmdptr<MyCommand> firstCommand = uvl::mkcmd<MyCommand>("Hello,world!");

  return 0;
}
