#pragma once

#include <forward_list>
#include <stack>

#include "uvlib/command.hpp"

namespace uvlib {
class Scheduler {
   private:
    std::forward_list<std::stack<Command>> scheduled_commands;

   public:
    void add_command(Command command);

    std::forward_list<std::stack<Command>> get_scheduled_commands();
};
}  // namespace uvlib