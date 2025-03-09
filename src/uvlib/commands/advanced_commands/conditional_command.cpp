#include "uvlib/commands/advanced_commands/conditional_command.hpp"
#include "uvlib/scheduler.hpp"

namespace uvl {
ConditionalCommand::ConditionalCommand(std::function<bool()> conditional,
                                       CommandPtr &&if_true,
                                       CommandPtr &&if_false)
    : m_conditional(std::move(conditional)), m_if_true(std::move(if_true)),
      m_if_false(std::move(if_false)) {}

bool ConditionalCommand::is_finished() { return m_executed; }

void ConditionalCommand::initialize() {
  Scheduler &scheduler = Scheduler::get_instance();

  if (m_conditional()) {
    scheduler.schedule_command(m_if_true.get());
  } else {
    scheduler.schedule_command(m_if_false.get());
  }

  m_executed = true;
}

void ConditionalCommand::end(bool interrupted) { m_executed = false; }

} // namespace uvl