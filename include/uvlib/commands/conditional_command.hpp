#pragma once

#include <functional>

#include "uvlib/commands/command.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {
/**
 * Executes the required action the next tick
 * by the scheduler.
 */
class ConditionalCommand : public Command {
 private:
  const std::function<void()>& condition_lambda;

 public:
  ConditionalCommand(const std::function<bool()>& condition_lambda,
                     const init_subsystems_t& requirements);

  void execute() override;

  bool is_finished() override;
};
}  // namespace uvl