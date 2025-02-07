#pragma once

namespace uvlib {

class Command {
 private:
 public:
  virtual void initialize();

  virtual void execute();

  virtual bool is_finished() = 0;

  virtual void end(bool interrupted);
};

}  // namespace uvlib