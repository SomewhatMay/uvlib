#include "main.h"
#include "uvlib/command-api.hpp"
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
}
