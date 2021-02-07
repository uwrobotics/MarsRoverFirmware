#pragma once

#include "BasicControl.h"

namespace Controller {

class Velocity final : public BasicControl {
 public:
  using BasicControl::BasicControl;
  void update() override;
};
}  // namespace Controller
