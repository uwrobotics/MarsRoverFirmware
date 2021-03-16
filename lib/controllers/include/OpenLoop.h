#pragma once

#include "ActuatorController.h"

namespace Controller {
class OpenLoop final : public ActuatorController {
 public:
  using ActuatorController::ActuatorController;

  void stop() override;
  void reset() override;
  bool update() override;

  std::optional<std::reference_wrapper<PID::PID>> getPID() override;
};
}  // namespace Controller
