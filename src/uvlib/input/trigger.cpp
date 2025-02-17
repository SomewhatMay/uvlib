
#include "uvlib/input/trigger.hpp"

#include "main.h"
#include "uvlib/commands/command.hpp"
#include "uvlib/enums.hpp"
#include "uvlib/scheduler.hpp"

namespace uvl {
Trigger::Trigger(pros::Controller *controller, TriggerButton button)
    : controller(controller), button(button) {}

Trigger::~Trigger() { unbind_all(); }

/* Execute */

constexpr pros::controller_digital_e_t to_pros_digital(TriggerButton button) {
  return static_cast<pros::controller_digital_e_t>(button);
}

bool Trigger::poll() {
  return controller->get_digital(to_pros_digital(button));
}

void Trigger::execute() {
  bool current_state = controller->get_digital(to_pros_digital(button));

  /* trigger: on_true */
  if (current_state && !previous_state && m_on_true) {
    Scheduler::get_instance().schedule_command(m_on_true->get());
  }

  /* trigger: on_false */
  if (!current_state && previous_state && m_on_false) {
    Scheduler::get_instance().schedule_command(m_on_false->get());
  }

  /* trigger: on_change */
  if (current_state != previous_state && m_on_change) {
    Scheduler::get_instance().schedule_command(m_on_change->get());
  }

  /* trigger: while_true */
  if (m_while_true) {
    if (current_state && !(*m_while_true)->m_is_alive) {
      Scheduler::get_instance().schedule_command(m_while_true->get());
    } else if (!current_state && (*m_while_true)->m_is_alive) {
      (*m_while_true)->cancel();
    }
  }

  /* trigger: while_false */
  if (m_while_false) {
    if (!current_state && !(*m_while_false)->m_is_alive) {
      Scheduler::get_instance().schedule_command(m_while_false->get());
    } else if (current_state && (*m_while_false)->m_is_alive) {
      (*m_while_false)->cancel();
    }
  }

  previous_state = current_state;
}

/* Binding Trigger Methods */

inline void replace_command(std::optional<CommandPtr> *location,
                            CommandPtr &&command) {
  if (*location) {
    (*(*location))->cancel();
  }

  *location = std::move(command);
}

Trigger &Trigger::on_true(CommandPtr &&command) {
  replace_command(&m_on_true, std::move(command));

  return *this;
}

Trigger &Trigger::on_false(CommandPtr &&command) {
  replace_command(&m_on_false, std::move(command));

  return *this;
}

Trigger &Trigger::on_change(CommandPtr &&command) {
  replace_command(&m_on_change, std::move(command));

  return *this;
}

Trigger &Trigger::while_true(CommandPtr &&command) {
  replace_command(&m_while_true, std::move(command));

  return *this;
}

Trigger &Trigger::while_false(CommandPtr &&command) {
  replace_command(&m_while_false, std::move(command));

  return *this;
}

/* Unbinding Trigger Methods */

inline void replace_command(std::optional<CommandPtr> *location) {
  if (*location) {
    (*(*location))->cancel();
  }

  location->reset();
}

Trigger &Trigger::on_true(std::nullopt_t) {
  replace_command(&m_on_true);
  return *this;
}

Trigger &Trigger::on_false(std::nullopt_t) {
  replace_command(&m_on_false);
  return *this;
}

Trigger &Trigger::on_change(std::nullopt_t) {
  replace_command(&m_on_change);
  return *this;
}

Trigger &Trigger::while_true(std::nullopt_t) {
  replace_command(&m_while_true);
  return *this;
}

Trigger &Trigger::while_false(std::nullopt_t) {
  replace_command(&m_while_false);
  return *this;
}

Trigger &Trigger::unbind_all() {
  on_true(std::nullopt);
  on_false(std::nullopt);
  on_change(std::nullopt);
  while_true(std::nullopt);
  while_false(std::nullopt);

  return *this;
}

} // namespace uvl