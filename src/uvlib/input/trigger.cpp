
#include "uvlib/input/trigger.hpp"

#include "main.h"
#include "uvlib/commands/command.hpp"
#include "uvlib/scheduler.hpp"

namespace uvl {
explicit Trigger::Trigger(pros::Controller* controller, TriggerButton button)
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

}  // namespace uvl