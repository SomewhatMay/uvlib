#pragma once

#include <forward_list>
#include <list>
#include <memory>

#include "main.h"

namespace uvl {
class Subsystem;
class Command;
class CommandPtr;
class Trigger;
class Joystick;

template <class Derived, class Base>
concept DecayedDerivedFrom =
    std::derived_from<std::decay_t<Derived>, std::decay_t<Base>> &&
    std::convertible_to<std::decay_t<Derived>*, std::decay_t<Base>*>;

}  // namespace uvl