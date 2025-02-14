#include "uvlib/commands/instant_command.hpp"

namespace uvl {
InstantCommand::InstantCommand(const std::function<void()>& instant_action,
                               const init_subsystems_t& requirements)
    : instant_action(instant_action) {
  set_requirements(requirements);
}

void InstantCommand::execute() {
  executed = true;
  instant_action();
}

bool InstantCommand::is_finished() { return executed; }

}  // namespace uvl