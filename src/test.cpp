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
  // std::shared_ptr<MyCommand> secondCommand(new MyCommand());

  // uvl::cmdptr<MyCommand> command(new MyCommand());
  // uvl::cmdptr<MyCommand> command = uvl::mkcmd<MyCommand>();

  // MyCommand command = MyCommand();

  // myCommand->and_then(uvl::mkcmd<MyCommand>());
  // myCommand->and_then(std::make_shared<MyCommand>());
  // myCommand->and_then<MyCommand>("Hello, world!");

  // myCommand->race_with({uvl::mkcmd<MyCommand>(), uvl::mkcmd<MyCommand>(),
  //                       uvl::mkcmd<MyCommand>(),
  //                       uvl::mkcmd<MyCommand>()});
}
