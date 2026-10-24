#include <cstring>

#include "TutorialServo.h"
#include "mbed.h"

namespace {
constexpr uint32_t CAN_FREQUENCY_HZ  = 500000;
constexpr uint16_t SERVO_POSITION_ID = 0x100;  // payload: float, percentage of servo range [0, 100]
constexpr uint16_t STANDARD_ID_MASK  = 0x7FF;
constexpr float MAX_PERCENTAGE       = 100.0f;
}  // namespace

CAN can(CAN1_RX, CAN1_TX, CAN_FREQUENCY_HZ);
TutorialServo servo(PA_1);

int main() {
  printf("tutorial-servo-can-control\r\n");

  can.filter(SERVO_POSITION_ID, STANDARD_ID_MASK, CANStandard);

  CANMessage msg;
  while (true) {
    if (can.read(msg) && msg.id == SERVO_POSITION_ID && msg.len >= sizeof(float)) {
      float percentage = 0.0f;
      std::memcpy(&percentage, msg.data, sizeof(float));  // little-endian IEEE-754, matches STM32

      const float degrees = percentage / MAX_PERCENTAGE * servo.getServoRangeInDegrees();
      servo.setPositionInDegrees(degrees);

      printf("Servo set to %d%% (%d deg)\r\n", static_cast<int>(percentage), static_cast<int>(degrees));
    }
    ThisThread::sleep_for(1ms);
  }
}
