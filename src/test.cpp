#include <functional>
#include <initializer_list>
#include <memory>

#include "main.h"
#include "uvlib/command_api.hpp"
#include "uvlib/input_api.hpp"
#include "uvlib/scheduler.hpp"
#include "uvlib/subsystem.hpp"
#include "uvlib/typedefs.hpp"

class MySubsystem : public uvl::Subsystem {};

class MyCommand : public uvl::Command {
 public:
  MyCommand() = default;

  MyCommand(std::string greeting) { std::cout << greeting << '\n'; }

  MyCommand(const std::function<int()>& greet,
            const uvl::init_subsystems_t& subsystems) {
    greet();
    // std::cout << greeting << '\n';

    set_requirements(subsystems);
  }

  bool is_finished() { return false; }
};

int test() {
  MySubsystem* mySubsystem = new MySubsystem();

  uvl::cmdptr<MyCommand> myCommand(new MyCommand());
  uvl::cmdptr<MyCommand> secondCommand(new MyCommand());

  uvl::Controller master(pros::E_CONTROLLER_MASTER);
  uvl::Trigger& L1 = master.get_trigger(uvl::TriggerButton::kL1);
  L1.on_true<MyCommand>();

  L1.on_false(uvl::mkcmd<MyCommand>()
                  ->and_then<MyCommand>()
                  ->on_interrupted<MyCommand>());

  // L1.on_change(uvl::mkcmd<uvl::ConditionalCommand>(
  //                  []() { return true; },
  //                  uvl::init_subsystems_t{mySubsystem})
  //                  ->and_then<MyCommand>());

  // MyCommand("Hello, world", mySubsystem);

  // uvl::mkcmd<uvl::ConditionalCommand>([]() { return true; },
  //                                     uvl::init_subsystems_t{mySubsystem});

  std::make_shared<MyCommand>([]() { return true; },
                              uvl::init_subsystems_t{mySubsystem});

  uvl::mkcmd<uvl::ConditionalCommand>([]() { return true; },
                                      uvl::init_subsystems_t{mySubsystem});

  myCommand->and_then(secondCommand)->and_then<MyCommand>();

  uvl::cmdptr<MyCommand> firstCommand = uvl::mkcmd<MyCommand>("Hello,world!");

  return 0;
}
