#include "uvlib/subsystem.hpp"

#include "uvlib/scheduler.hpp"

namespace uvlib {
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

template <typename Derived_Command, typename... Args>
std::enable_if_t<std::is_base_of_v<Command, Derived_Command>,
                 std::shared_ptr<Derived_Command>>
Subsystem::set_default_command(Args&&... constructor_args) {
  std::shared_ptr<Derived_Command> command = std::make_shared<Derived_Command>(
      std::forward<Args>(constructor_args)...);

  set_default_command(command);

  return command;
}

bool Subsystem::get_used_current_tick() const { return used_current_tick; }

void Subsystem::set_used_current_tick(bool used_current_tick) {
  this->used_current_tick = used_current_tick;
}
}  // namespace uvlib