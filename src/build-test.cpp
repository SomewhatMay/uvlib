/**
 * Test any templated functions/classes to ensure they properly compile.
 */

#include "main.h"
#include "uvlib/command_api.hpp"
#include "uvlib/enums.hpp"
#include "uvlib/input_api.hpp"
#include "uvlib/scheduler.hpp"
#include "uvlib/subsystem.hpp"
#include "uvlib/typedefs.hpp"

int test_main() {
  uvl::Controller master(pros::E_CONTROLLER_MASTER);

  master.get_trigger(uvl::TriggerButton::kA)
      .on_change(uvl::FunctionCommand([]() {}, []() {}, []() { return true; },
                                      [](bool interrupted) {})
                     .to_ptr());

  return 0;
}