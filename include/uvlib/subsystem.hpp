#pragma once

#include "uvlib/command.hpp"

namespace uvlib {
class Subsystem {
 private:
 public:
  Subsystem();

  void set_default_command(Command command);

  void run(Command command);

  virtual void initialize();

  virtual void periodic();
};
}  // namespace uvlib