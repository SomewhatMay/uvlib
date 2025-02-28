#pragma once

#include <concepts>
#include <memory>
#include <type_traits>

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/commandptr.hpp"

namespace uvl {
/**
 * Wrap any custom commands with this class to ensure they behave appropriately
 * and can be used throughout uvlib.
 */
template <typename Base, typename CRTP> class CommandHelper : public Base {
  static_assert(std::is_base_of_v<Command, Base>,
                "Base must derive from Command");
  using Base::Base;

public:
  CommandHelper() = default;

  virtual ~CommandHelper() = default;

  CommandHelper(CommandHelper &&) = default;
  CommandHelper &operator=(CommandHelper &&) = default;

  CommandPtr to_ptr() && override {
    return CommandPtr(
        std::make_unique<CRTP>(std::move(*static_cast<CRTP *>(this))));
  }
};
} // namespace uvl