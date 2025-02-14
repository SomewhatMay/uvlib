#include "uvlib/commands/conditional_command.hpp"

namespace uvl {
// ConditionalCommand::ConditionalCommand(
//     const std::function<bool()>& condition_lambda,
//     const init_subsystems_t& requirements)
//     : condition_lambda(condition_lambda) {
//   set_requirements(requirements);
// }

void ConditionalCommand::execute() { condition_lambda(); }

bool ConditionalCommand::is_finished() { return false; }

}  // namespace uvl