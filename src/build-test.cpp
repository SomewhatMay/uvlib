/**
 * Test any templated functions/classes to ensure they properly compile.
 */

#include <iostream>
#include <optional>

#include "main.h"
#include "uvlib/command_api.hpp"
#include "uvlib/commands/advanced_commands/instant_command.hpp"
#include "uvlib/commands/advanced_commands/wait_command.hpp"
#include "uvlib/commands/advanced_commands/wait_until_command.hpp"
#include "uvlib/enums.hpp"
#include "uvlib/input_api.hpp"
#include "uvlib/scheduler.hpp"
#include "uvlib/subsystem.hpp"
#include "uvlib/typedefs.hpp"

int test_main() {
  uvl::Controller master(pros::E_CONTROLLER_MASTER);

  master.get_trigger(uvl::TriggerButton::kA)
      .on_change(uvl::FunctionCommand([]() {}, []() {}, []() { return true; },
                                      [](bool interrupted) {}, {})
                     .to_ptr());

  master.get_trigger(uvl::TriggerButton::kB)
      .on_true(uvl::SequentialCommandGroup(
                   uvl::InstantCommand(
                       []() { std::cout << "First command executed\n"; }, {})
                       .to_ptr(),
                   uvl::InstantCommand(
                       []() { std::cout << "Second command executed\n"; }, {})
                       .to_ptr())
                   .to_ptr());

  master.get_trigger(uvl::TriggerButton::kX)
      .on_true(
          uvl::InstantCommand([]() { std::cout << "First command executed\n"; },
                              {})
              .and_then(
                  uvl::InstantCommand(
                      []() { std::cout << "Second command executed!\n"; }, {})
                      .to_ptr())
              .and_then(
                  uvl::InstantCommand(
                      []() { std::cout << "Third command executed\n"; }, {})
                      .to_ptr()));

  master.get_trigger(uvl::TriggerButton::kY)
      .on_true(uvl::WaitCommand(3000).and_then(
          uvl::InstantCommand(
              []() { std::cout << "Command executed 3 seconds later!\n"; }, {})
              .to_ptr()));

  master.get_trigger(uvl::TriggerButton::kA).on_false(std::nullopt);

  master.get_trigger(uvl::TriggerButton::kB)
      .on_false(
          uvl::WaitUntilCommand([left_joystick = master.left_joystick()]() {
            return left_joystick.get_x() != 0;
          })
              .and_then(uvl::InstantCommand(
                            []() { std::cout << "Joystick moved!\n"; }, {})
                            .to_ptr()));

  return 0;
}