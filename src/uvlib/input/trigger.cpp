
#include "uvlib/input/trigger.hpp"

#include "main.h"
#include "uvlib/commands/command.hpp"
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
    Scheduler::get_instance().schedule_command(*m_on_true);
  }

  /* trigger: on_false */
  if (m_on_false && !current_state && previous_state) {
    Scheduler::get_instance().schedule_command(*m_on_false);
  }

  /* trigger: on_change */
  if (m_on_change && current_state != previous_state) {
    Scheduler::get_instance().schedule_command(*m_on_change);
  }

  /* trigger: while_true */
  if (m_while_true) {
    if (current_state && !(*m_while_true)->get_alive()) {
      Scheduler::get_instance().schedule_command(*m_while_true);
    } else if (!current_state && (*m_while_true)->get_alive()) {
      (*m_while_true)->cancel();
    }
  }

  /* trigger: while_false */
  if (m_while_false) {
    if (!current_state && !(*m_while_false)->get_alive()) {
      Scheduler::get_instance().schedule_command(*m_while_false);
    } else if (current_state && (*m_while_false)->get_alive()) {
      (*m_while_false)->cancel();
    }
  }

  previous_state = current_state;
}

/* Trigger Methods */

void replace_command(std::optional<commandptr_t>* location,
                     commandptr_t command) {
  if (*location) {
    (*(*location))->cancel();
  }

  *location = command;
}

/* trigger: on_true */
Trigger& Trigger::on_true(cmdptr<Command> command) {
  /*
  cancel ongoing command
  if (m_on_true) {
    (*m_on_true)->cancel();
  }

  m_on_true = command;
  */

  replace_command(&m_on_true, command);

  return *this;
}

template <typename DerivedCommand, typename... Args>
Trigger& Trigger::on_true(Args&&... constructor_args) {
  replace_command(&m_on_true, std::make_shared<DerivedCommand>(
                                  std::forward<Args>(constructor_args)...));

  return *this;
}

/* trigger: on_false */
Trigger& Trigger::on_false(cmdptr<Command> command) {
  replace_command(&m_on_false, command);

  return *this;
}

template <typename DerivedCommand, typename... Args>
Trigger& Trigger::on_false(Args&&... constructor_args) {
  replace_command(&m_on_false, std::make_shared<DerivedCommand>(
                                   std::forward<Args>(constructor_args)...));

  return *this;
}

/* trigger: on_change */
Trigger& Trigger::on_change(cmdptr<Command> command) {
  replace_command(&m_on_change, command);

  return *this;
}

template <typename DerivedCommand, typename... Args>
Trigger& Trigger::on_change(Args&&... constructor_args) {
  replace_command(&m_on_change, std::make_shared<DerivedCommand>(
                                    std::forward<Args>(constructor_args)...));

  return *this;
}

/* trigger: while_true */
Trigger& Trigger::while_true(cmdptr<Command> command) {
  replace_command(&m_while_true, command);

  return *this;
}

template <typename DerivedCommand, typename... Args>
Trigger& Trigger::while_true(Args&&... constructor_args) {
  replace_command(&m_while_true, std::make_shared<DerivedCommand>(
                                     std::forward<Args>(constructor_args)...));

  return *this;
}

/* trigger: while_false */
Trigger& Trigger::while_false(cmdptr<Command> command) {
  replace_command(&m_while_false, command);

  return *this;
}

template <typename DerivedCommand, typename... Args>
Trigger& Trigger::while_false(Args&&... constructor_args) {
  replace_command(&while_false, std::make_shared<DerivedCommand>(
                                    std::forward<Args>(constructor_args)...));

  return *this;
}

}  // namespace uvl