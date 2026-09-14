#include <cmath>
#include <cstring>

#include "TutorialServo.h"
#include "mbed.h"

constexpr unsigned int kServoCommandId = 0x100;
constexpr int kCanFrequencyHz          = 500000;

CAN can(CAN_RX, CAN_TX, kCanFrequencyHz);
TutorialServo servo(PA_1);

int main() {
  static_assert(sizeof(float) == 4, "The CAN command uses a 32 bit float");
  const int filterResult = can.filter(kServoCommandId, 0x7FF, CANStandard);
  MBED_ASSERT(filterResult != 0);
  CANMessage message;

  while (true) {
    while (can.read(message)) {
      if (message.id == kServoCommandId && message.type == CANData && message.format == CANStandard &&
          message.len == sizeof(float)) {
        // The STM32F446 and the command payload both use little endian byte order.
        float percentage;
        std::memcpy(&percentage, message.data, sizeof(percentage));
        if (std::isfinite(percentage) && percentage >= 0.0f && percentage <= 100.0f) {
          servo.setPositionInDegrees(percentage * servo.getServoRangeInDegrees() / 100.0f);
        }
      }
    }
    ThisThread::sleep_for(1ms);
  }
}
