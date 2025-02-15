
#include "uvlib/input/trigger.hpp"

#include "main.h"
#include "uvlib/commands/command.hpp"
#include "uvlib/enums.hpp"
#include "uvlib/scheduler.hpp"

namespace uvl {
Trigger::Trigger(pros::Controller* controller, TriggerButton button)
    : controller(controller), button(button) {}

/* Execute */

constexpr pros::controller_digital_e_t to_pros_digital(TriggerButton button) {
  return static_cast<pros::controller_digital_e_t>(button);
}

void Trigger::execute() {
  bool current_state = controller->get_digital(to_pros_digital(button));

  /* trigger: on_true */
  if (m_on_true && current_state && !previous_state) {
    Scheduler::get_instance().schedule_command(std::move(*m_on_true));
  }

  /* trigger: on_false */
  if (m_on_false && !current_state && previous_state) {
    Scheduler::get_instance().schedule_command(std::move(*m_on_false));
  }

  /* trigger: on_change */
  if (m_on_change && current_state != previous_state) {
    Scheduler::get_instance().schedule_command(std::move(*m_on_change));
  }

  /* trigger: while_true */
  if (m_while_true) {
    if (current_state && !(*m_while_true)->m_is_alive) {
      Scheduler::get_instance().schedule_command(std::move(*m_while_true));
    } else if (!current_state && (*m_while_true)->m_is_alive) {
      (*m_while_true)->cancel();
    }
  }

  /* trigger: while_false */
  if (m_while_false) {
    if (!current_state && !(*m_while_false)->m_is_alive) {
      Scheduler::get_instance().schedule_command(std::move(*m_while_false));
    } else if (current_state && (*m_while_false)->m_is_alive) {
      (*m_while_false)->cancel();
    }
  }

  previous_state = current_state;
}

/* Trigger Methods */

inline void replace_command(std::optional<CommandPtr>* location,
                            CommandPtr&& command) {
  if (*location) {
    (*(*location))->cancel();
  }

  *location = std::move(command);
}

Trigger& Trigger::on_true(CommandPtr&& command) {
  replace_command(&m_on_true, std::move(command));

  return *this;
}

Trigger& Trigger::on_false(CommandPtr&& command) {
  replace_command(&m_on_false, std::move(command));

  return *this;
}

Trigger& Trigger::on_change(CommandPtr&& command) {
  replace_command(&m_on_change, std::move(command));

  return *this;
}

Trigger& Trigger::while_true(CommandPtr&& command) {
  replace_command(&m_while_true, std::move(command));

  return *this;
}

Trigger& Trigger::while_false(CommandPtr&& command) {
  replace_command(&m_while_false, std::move(command));

  return *this;
}

}  // namespace uvl