#pragma once

#include <list>
#include <memory>

namespace uvlib {
class Subsystem;
class Command;

using command_chain_t = std::list<std::shared_ptr<Command>>;
using command_list_t = std::list<command_chain_t>;
using subsystem_list_t = std::list<Subsystem *>;

template <typename T>
using constructable_command_t =
    std::enable_if_t<std::is_base_of_v<Command, T>, std::shared_ptr<T>>;
}  // namespace uvlib