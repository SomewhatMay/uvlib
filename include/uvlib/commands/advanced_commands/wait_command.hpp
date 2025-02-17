#pragma once

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"

namespace uvl {
class WaitCommand : public CommandHelper<Command, WaitCommand> {
public:
  /**
   * @brief Construct a new Wait Command that pauses for duration miliseconds
   * before marking itself as finished. Useful for command chaining based on
   * timestamps.
   *
   * @note Since this command does not have any requirements, it can never be
   * interrupted when scheduling another command, even if the other command is
   * another WaitCommand!
   *
   * @param duration The duration, in miliseconds, this command should wait for
   * before returning true in is_finished.
   */
  WaitCommand(double duration);

protected:
  bool is_finished() override;

private:
  /**
   * @brief The timestamp to wait until this command marks itself as finished.
   */
  double m_end_timestamp;
};

} // namespace uvl