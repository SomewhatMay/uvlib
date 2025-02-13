#pragma once

#include <list>
#include <memory>

namespace uvlib {

class Subsystem;
class Command;

/**
 * Determines the order that a command is added
 * to the list of commands that will be executed
 * by the scheduler. The scheduler executes commands
 * from top to bottom; therefore, by default, adding
 * commands to the list behaves like a stack.
 */
enum ScheduleDirection {
  TOP = 0,  // DEFAULT
  BOTTOM = 1
};

using commandptr_t = std::shared_ptr<Command>;
using command_list_t = std::list<commandptr_t>;
using subsystem_list_t = std::list<Subsystem *>;

template <typename Derived_Command>
using constructable_command_t =
    std::enable_if_t<std::is_base_of_v<Command, Derived_Command>,
                     std::shared_ptr<Derived_Command>>;

}  // namespace uvlib