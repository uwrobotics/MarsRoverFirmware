#include "CANConfig.h"
#include "CANInterface.h"
#include "TutorialServo.h"
#include "hw_bridge.h"

CANInterface can(CANConfig::config);
TutorialServo servo(PA_1);

int main() {
    printf("Tutorial servo CAN control\r\n");

    can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

    while (true) {
        HWBRIDGE::CANSignalValue_t percentValue = 0;
        can.getRXSignalValue(HWBRIDGE::CANID::COMMON_DEBUG_MESSAGE1, HWBRIDGE::CANSIGNAL::COMMON_DEBUG_SIGNAL1, percentValue);

        float percent = static_cast<float>(percentValue);
        float degrees = (percent / 100.0f) * servo.getServoRangeInDegrees();
        servo.setPositionInDegrees(degrees);

        printf("Received percent: %.1f -> degrees: %.1f\r\n", percent, degrees);

        ThisThread::sleep_for(100ms);
    }
}