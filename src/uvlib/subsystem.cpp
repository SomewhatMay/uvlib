#include "uvlib/subsystem.hpp"

#include "uvlib/scheduler.hpp"

namespace uvl {
void Subsystem::register_self() {
  Scheduler::get_instance().register_subsystem(this);
}

Subsystem::Subsystem() { register_self(); }

void Subsystem::initialize() {}

void Subsystem::periodic() {}

/* Getters and Setters */

cmdptr<Command> Subsystem::get_default_command() { return default_command; }

void Subsystem::set_default_command(cmdptr<Command> command) {
  this->default_command = command;
}

bool Subsystem::get_used_current_tick() const { return used_current_tick; }

void Subsystem::set_used_current_tick(bool used_current_tick) {
  this->used_current_tick = used_current_tick;
}
}  // namespace uvl