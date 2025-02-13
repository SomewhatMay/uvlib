#include "main.h"
#include "uvlib/command-api.hpp"
#include "uvlib/typedefs.hpp"

class MyCommand : public uvlib::Command {
 public:
  MyCommand() = default;

  MyCommand(std::string greeting) { std::cout << greeting << '\n'; }

  bool is_finished() { return false; }
};

int test() {
  uvlib::cmdptr<MyCommand> myCommand(new MyCommand());
  // std::shared_ptr<MyCommand> secondCommand(new MyCommand());

  // uvlib::cmdptr<MyCommand> command(new MyCommand());
  // uvlib::cmdptr<MyCommand> command = uvlib::mkcmd<MyCommand>();

  // MyCommand command = MyCommand();

  // myCommand->and_then(uvlib::mkcmd<MyCommand>());
  // myCommand->and_then(std::make_shared<MyCommand>());
  // myCommand->and_then<MyCommand>("Hello, world!");

  // myCommand->race_with({uvlib::mkcmd<MyCommand>(), uvlib::mkcmd<MyCommand>(),
  //                       uvlib::mkcmd<MyCommand>(),
  //                       uvlib::mkcmd<MyCommand>()});
}
