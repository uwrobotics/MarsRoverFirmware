#pragma once

#include "BasicControl.h"

namespace Controller {

class Current final : public BasicControl {
 public:
  using BasicControl::BasicControl;
  void update() override;
};
}  // namespace Controller
