#include "uvlib/subsystem.hpp"

#include "uvlib/scheduler.hpp"

namespace uvl {
void Subsystem::register_self() { get_scheduler().register_subsystem(this); }

Subsystem::Subsystem() { register_self(); }

void Subsystem::initialize() {}

void Subsystem::periodic() {}

/* Getters and Setters */

std::shared_ptr<Command>& Subsystem::get_default_command() {
  return default_command;
}

void Subsystem::set_default_command(std::shared_ptr<Command> command) {
  this->default_command = command;
}

template <typename DerivedCommand, typename... Args>
cmdptr<DerivedCommand> Subsystem::set_default_command(
    Args&&... constructor_args) {
  std::shared_ptr<DerivedCommand> command =
      std::make_shared<DerivedCommand>(std::forward<Args>(constructor_args)...);

  set_default_command(command);

  return command;
}

bool Subsystem::get_used_current_tick() const { return used_current_tick; }

void Subsystem::set_used_current_tick(bool used_current_tick) {
  this->used_current_tick = used_current_tick;
}
}  // namespace uvl