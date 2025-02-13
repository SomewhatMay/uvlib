#include "uvlib/command.hpp"

#include "uvlib/scheduler.hpp"

namespace uvlib {

void Command::initialize() {}

void Command::execute() {}

void Command::end(bool interrupted) {}

/**
 * Checks if a specific command chain exists,
 * and if it does, schedules every command in it,
 * concluding by resetting the command chain to an
 * empty state.
 */
void schedule_chained_commands(
    std::optional<std::list<commandptr_t>>* chained_command) {
  Scheduler& scheduler = get_scheduler();

  if (chained_command) {
    for (auto& new_command : *(*chained_command)) {
      scheduler.schedule_command(new_command);
    }
  }

  chained_command->reset();
}

void Command::on_end(bool interrupted) {
  end(interrupted);

  if (!interrupted) {
    schedule_chained_commands(&and_then_commands);
  } else {
    schedule_chained_commands(&catch_commands);
  }

  schedule_chained_commands(&finally_commands);
}

void Command::cancel() {
  set_alive(false);
  on_end(false);
}

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