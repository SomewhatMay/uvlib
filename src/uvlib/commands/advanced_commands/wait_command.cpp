#include "uvlib/commands/advanced_commands/wait_command.hpp"
#include "pros/rtos.hpp"

namespace uvl {
WaitCommand::WaitCommand(double duration)
    : m_end_timestamp(pros::millis() + duration) {}

bool WaitCommand::is_finished() { return pros::millis() >= m_end_timestamp; }
} // namespace uvl