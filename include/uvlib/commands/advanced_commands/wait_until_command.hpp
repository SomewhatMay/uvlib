#pragma once

#include "uvlib/commands/command.hpp"
#include "uvlib/commands/command_helper.hpp"
#include "uvlib/subsystem.hpp"
#include <functional>
#include <initializer_list>

namespace uvl {
class WaitUntilCommand : public CommandHelper<Command, WaitUntilCommand> {
public:
  WaitUntilCommand(std::function<bool()> condition, uint32_t timeout,
                   std::initializer_list<Subsystem *> requirements = {});

  WaitUntilCommand(std::function<bool()> condition,
                   std::initializer_list<Subsystem *> requirements)
      : WaitUntilCommand(std::move(condition), 100000000,
                         std::move(requirements)) {}

  WaitUntilCommand(std::function<bool()> condition)
      : WaitUntilCommand(std::move(condition), 100000000) {}

  bool is_finished() override;

private:
  std::function<bool()> m_condition;
  uint32_t m_timeout_end;
};
} // namespace uvl