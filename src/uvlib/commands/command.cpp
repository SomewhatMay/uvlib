#include "uvlib/commands/command.hpp"

#include "uvlib/scheduler.hpp"
#include "uvlib/typedefs.hpp"

namespace uvl {

void Command::initialize() {}

void Command::execute() {}

void Command::end(bool interrupted) {}

/**
 * Checks if a specific command chain exists,
 * and if it does, schedules every command in it.
 * It does not reset the chain!
 */
void schedule_chained_commands(
    std::optional<std::forward_list<commandptr_t>>* chained_command) {
  Scheduler& scheduler = Scheduler::get_instance();

  if (*chained_command) {
    for (auto& new_command : *(*chained_command)) {
      scheduler.schedule_command(new_command);
    }
  }

  // chained_command->reset();
}

void Command::on_end(bool interrupted) {
  end(interrupted);

  if (!interrupted) {
    schedule_chained_commands(&and_then_commands);
  } else {
    schedule_chained_commands(&on_interrupted_commands);
  }

  schedule_chained_commands(&finally_commands);
}

void Command::cancel() {
  if (is_alive) {
    set_alive(false);
    on_end(false);
  }
}

/* Command Composition */

/* composition: and_then */
cmdptr<Command> Command::and_then(cmdptr<Command> command) {
  and_then_commands->push_front(command);
  return command;
}

/* composition: on_interrupted */
cmdptr<Command> Command::on_interrupted(cmdptr<Command> command) {
  on_interrupted_commands->push_front(command);
  return command;
}

/* composition: finally */
cmdptr<Command> Command::finally(cmdptr<Command> command) {
  finally_commands->push_front(command);
  return command;
}

/* Getters and Setters */

void Command::set_requirements(const init_subsystems_t& subsystems) {
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

ScheduleDirection Command::get_schedule_direction() const {
  return scheduleDirection;
}

void Command::set_schedule_direction(ScheduleDirection direction) {
  scheduleDirection = direction;
}

int Command::get_tick_number() const { return tick_number; }

}  // namespace uvl