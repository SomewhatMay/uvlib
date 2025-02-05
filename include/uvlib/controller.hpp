#pragma once

#include "main.h"

namespace uvlib {
class Controller {
   protected:
    pros::Controller controller;

   public:
    Controller(pros::controller_id_e_t id) : controller(id) {
    }

    pros::Controller get_controller() {
        return controller;
    }
};
}  // namespace uvlib