#pragma once

#include "BasicControl.h"

namespace Controller {

class Position final : public BasicControl {
 public:
  using BasicControl::BasicControl;
  void stop() override final;
  bool update() override;
};
}  // namespace Controller
