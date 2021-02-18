#pragma once

#include "BasicControl.h"

namespace Controller {

class Velocity final : public BasicControl {
 public:
  using BasicControl::BasicControl;
  void stop() override final;
  bool update() override;
};
}  // namespace Controller
