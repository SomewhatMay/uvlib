#pragma once

#include <functional>

#include "uvlib/commands/command.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * Executes the required action the next tick
 * by the scheduler.
 */
class InstantCommand : public Command {
 private:
  bool executed = false;

  const std::function<void()>& instant_action;

 public:
  InstantCommand(const std::function<void()>& instant_action,
                 const initializer_subsystems_t& requirements);

  void execute() override;

  bool is_finished() override;
};
}  // namespace uvl