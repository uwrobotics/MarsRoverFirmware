#include "ActuatorController.h"
#include "CANMsg.h"
#include "EncoderAbsolute_PWM.h"
#include "hw_bridge.h"
#include "testPIDTuningApi.h"

// Context: This file tests the PID tuning over CAN functionality. This functionality is meant to be used when
// experimenting
//          with different PID Parameters.
// Setup: This test can be run on any target. The target must be connected to the Jetson through CAN bus. Before PID
// parameters can be modified,
//        we must manually enable the allowPIDTuning bool by sending true to SET_PID_TUNING_MODE
// Description: The Jetson runs a ROS node which presents users with a GUI, which sends appropriate CAN msgs.
//              For details on how to run the ROS node, refer to the Tuning PID API ROS Node.
// Test Execution: The success of this test is based upon visual inspection. The printed details should match the
// selection made on the GUI.

int main() {
  CAN can(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
  testPIDTuningApi testDriver;
  while (true) {
    CANMsg msg;
    if (can.read(msg)) {
      msg.id == HWBRIDGE::CANID::SET_PID_TUNING_MODE ? testDriver.setPIDTuningMode(msg)
                                                     : testDriver.setPIDParameter(msg);
      testDriver.printAllActuatorDetails();
    }
  }
}