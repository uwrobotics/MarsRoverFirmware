#include "CANConfig.h"
#include "CANInterface.h"
#include "Logger.h"
#include "TutorialServo.h"
#include "hw_bridge.h"
#include "mbed.h"

CANInterface can(CANConfig::config);
TutorialServo servo(PA_1);

int main() {
  Utility::logger << "";  // Band-aid fix for logger bug (issue #328)
  printf("Tutorial servo CAN control\r\n");

  // Only accept messages meant for all boards (debug messages live here)
  can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

  HWBRIDGE::CANSignalValue_t percentage = 0;

  while (true) {
    // Latest percentage received over CAN (0 to 100)
    can.getRXSignalValue(HWBRIDGE::CANID::COMMON_DEBUG_MESSAGE1, HWBRIDGE::CANSIGNAL::COMMON_DEBUG_SIGNAL1, percentage);

    // Same math as the pot app, the input just comes from CAN now
    float fraction = static_cast<float>(percentage) / 100.0f;
    servo.setPositionInDegrees(fraction * servo.getServoRangeInDegrees());

    ThisThread::sleep_for(20ms);
  }
}