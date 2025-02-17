#pragma once

#include <concepts>
#include <memory>

#include "uvlib/commands/command.hpp"

namespace uvl {
/**
 * Wrap around every command to ensure simple and effective memory management.
 * uvlib uses CommandPtr everywhere it can instead of raw pointers.
 */
class CommandPtr {
 public:
  explicit CommandPtr(std::unique_ptr<Command>&& command);

  template <std::derived_from<Command> T>
  // NOLINTNEXTLINE(bugprone-forwarding-reference-overload)
  explicit CommandPtr(T&& command)
      : CommandPtr(
            std::make_unique<std::decay_t<T>>(std::forward<T>(command))) {}

  explicit CommandPtr(std::nullptr_t) = delete;

  CommandPtr(CommandPtr&) = delete;
  CommandPtr& operator=(CommandPtr&) = delete;

  CommandPtr(CommandPtr&&);
  CommandPtr& operator=(CommandPtr&&) = default;

  /**
   * Directly access the internal Command pointer by dereferencing this class.
   */
  Command* operator*();
  Command* operator->();

  /**
   * Chains next with this command using the SequentialCommandGroup command.
   * Next is guaranteed to schedule when and if this command finishes
   * successfully.
   *
   * @note Invalidates the current CommandPtr and moves all its contents to
   * the returned CommandPtr as a SequentialCommandGroup.
   *
   * @param next The command to schedule after this one.
   *
   * @return The SequentialCommandGroup CommandPtr.
   */
  [[nodiscard]]
  CommandPtr and_then(CommandPtr&& next) &&;

  /**
   * Alias to CommandPtr::operator*()
   *
   * @return A pointer to the contained Command object.
   */
  Command* get() const&;

  /**
   * Disallow getting the internal command on an rvalue command since it would
   * be invalid anyways
   */
  Command* get() && = delete;

 private:
  std::unique_ptr<Command> m_ptr;
};
}  // namespace uvl
