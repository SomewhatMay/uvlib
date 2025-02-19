#include "uvlib/commands/command.hpp"

#include "uvlib/scheduler.hpp"

namespace uvl {
Command::~Command() { cancel(); }

void Command::cancel() { Scheduler::get_instance().cancel_command(this); }

CommandPtr Command::and_then(CommandPtr&& next) && {
  return std::move(*this).to_ptr().and_then(std::move(next));
}

void Command::on_end(bool interrupted) { end(interrupted); }

void Command::initialize() {}

void Command::execute() {}

bool Command::is_finished() { return false; }

void Command::end(bool interrupted) {}

void Command::add_requirements(std::initializer_list<Subsystem*> requirements) {
  for (auto subsystem : requirements) {
    m_requirements.push_back(subsystem);
  }
}

const std::list<Subsystem*>& Command::get_requirements() const {
  return m_requirements;
}

const bool& Command::is_alive() const { return m_is_alive; }

}  // namespace uvl