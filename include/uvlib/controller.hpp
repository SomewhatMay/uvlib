#pragma once

#include "main.h"

namespace uvl {
class Controller {
 protected:
  pros::Controller controller;

 public:
  Controller(pros::controller_id_e_t id) : controller(id) {}

  pros::Controller get_controller() { return controller; }
};
}  // namespace uvl