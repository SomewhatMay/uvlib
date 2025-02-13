#include "uvlib/commands/command.hpp"

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
    std::optional<std::forward_list<commandptr_t>>* chained_command) {
  Scheduler& scheduler = get_scheduler();

  if (chained_command) {
    for (auto& new_command : *(*chained_command)) {
      scheduler.schedule_command<Command>(new_command);
    }
  }

  chained_command->reset();
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
  set_alive(false);
  on_end(false);
}

/* Command Composition */

/* composition: and_then */
template <typename DerivedCommand>
cmdptr<DerivedCommand> Command::and_then(cmdptr<DerivedCommand> command) {
  and_then_commands->push_front(command);
  return command;
}

template <typename DerivedCommand, typename... Args>
cmdptr<DerivedCommand> and_then(Args&&... constructor_args) {
  cmdptr<DerivedCommand> command =
      std::make_shared<DerivedCommand>(std::forward<Args>(constructor_args)...);
  and_then(command);
  return command;
}

/* composition: on_interrupted */
template <typename DerivedCommand>
cmdptr<DerivedCommand> Command::on_interrupted(cmdptr<DerivedCommand> command) {
  on_interrupted_commands->push_front(command);
  return command;
}

template <typename DerivedCommand, typename... Args>
cmdptr<DerivedCommand> on_interrupted(Args&&... constructor_args) {
  cmdptr<DerivedCommand> command =
      std::make_shared<DerivedCommand>(std::forward<Args>(constructor_args)...);
  on_interrupted(command);
  return command;
}

/* composition: finally */
template <typename DerivedCommand>
cmdptr<DerivedCommand> Command::finally(cmdptr<DerivedCommand> command) {
  finally_commands->push_front(command);
  return command;
}

template <typename DerivedCommand, typename... Args>
cmdptr<DerivedCommand> finally(Args&&... constructor_args) {
  cmdptr<DerivedCommand> command =
      std::make_shared<DerivedCommand>(std::forward<Args>(constructor_args)...);
  finally(command);
  return command;
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

template <typename DerivedCommand, typename... Args>
cmdptr<DerivedCommand> mkcmd(Args&&... constructor_args) {
  cmdptr<DerivedCommand> command =
      std::make_shared<DerivedCommand>(std::forward<Args>(constructor_args)...);

  return std::static_pointer_cast<DerivedCommand>(command);
}

}  // namespace uvlib