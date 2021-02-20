#pragma once

#include "BasicControl.h"

namespace Controller {

class Current final : public BasicControl {
 public:
  using BasicControl::BasicControl;
  bool update() override;
  void stop() override final;
};
}  // namespace Controller
