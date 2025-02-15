#pragma once

#include <concepts>
#include <memory>

#include "uvlib/command/command.hpp"
#include "uvlib/command/commandptr.hpp"

namespace uvl {

template <std::derived_from<Command>, typename CRTP>
class CommandHelper : public Base {
  using Base::Base;

 public:
  CommandHelper() = default;

  CommandPtr to_ptr() && override {
    return CommandPtr(
        std::make_unique<CRTP>(std::move(*static_cast<CRTP*>(this))));
  }
};
}  // namespace uvl