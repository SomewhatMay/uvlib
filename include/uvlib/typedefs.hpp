#pragma once

#include <list>
#include <memory>

namespace uvl {

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
  kTop = 0,  // DEFAULT
  kBottom = 1
};

using commandptr_t = std::shared_ptr<Command>;

template <typename DerivedCommand>
using cmdptr = std::enable_if_t<std::is_base_of_v<Command, DerivedCommand>,
                                std::shared_ptr<DerivedCommand>>;

using command_list_t = std::list<commandptr_t>;
using subsystem_list_t = std::list<Subsystem *>;

}  // namespace uvl