#pragma once

#include <list>
#include <memory>

namespace uvlib {

class Subsystem;
class Command;

using command_chain_t = std::list<std::shared_ptr<Command>>;
using command_list_t = std::list<command_chain_t>;
using subsystem_list_t = std::list<Subsystem *>;
using commandptr_t = std::shared_ptr<Command>;

template <typename Derived_Command>
using constructable_command_t =
    std::enable_if_t<std::is_base_of_v<Command, Derived_Command>,
                     std::shared_ptr<Derived_Command>>;

}  // namespace uvlib