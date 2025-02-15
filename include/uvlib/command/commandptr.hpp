#pragma once

#include <concepts>
#include <memory>

#include "uvlib/command/command.hpp"

namespace uvl {
class CommandPtr {
 public:
  explicit CommandPtr(std::unique_ptr<Command>&& command);

  template <std::derived_from<Command> T>
  // NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
  explicit CommandPtr(T&& command)
      : CommandPtr(
            std::make_unique<std::decay_t<T>>(std::forward<T>(command))) {}

  explicit CommandPtr(std::nullptr_t) = delete;

  CommandPtr(CommandPtr&&);
  CommandPtr& operator=(CommandPtr&&) = default;

  Command* operator*();
  Command* operator->();

  Command* get() const&;

  /**
   * Disallow getting the internal command on
   * an rvalue command since it would be invalid
   * anyways
   */
  Command* get() && = delete;

 private:
  std::unique_ptr<Command> m_ptr;
};
}  // namespace uvl
