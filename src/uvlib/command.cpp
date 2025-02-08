#include "uvlib/command.hpp"

#include "scheduler.hpp"

namespace uvlib {

void Command::initialize() {}

void Command::execute() {}

void Command::end(bool interrupted) {}

void Command::cancel() { set_alive(false); }

/* Getters and Setters */

void Command::set_requirements(
    const std::initializer_list<Subsystem*>& subsystems) {
  requirements = subsystems;
}

const std::list<Subsystem*>& Command::get_requirements() const {
  return requirements;
}

bool Command::get_alive() const { return is_alive; }

void Command::set_alive(bool alive) { this->is_alive = alive; }

void Command::set_tick_number(int tick_number) {
  this->tick_number = tick_number;
}

int Command::get_tick_number() const { return tick_number; }

}  // namespace uvlib