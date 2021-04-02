#include "CANBus.h"
#include "CANMsg.h"
#include "ClawConfig.h"
#include "ElbowConfig.h"
#include "LimServo.h"
#include "Logger.h"
#include "ShoulderConfig.h"
#include "TooltipConfig.h"
#include "TurntableConfig.h"
#include "WristConfig.h"

// TODO: Add CAN handler for enabling/disabling current checks.
// TODO: Add CAN handler for enabling/disabling rpm checks
// TODO: Add CAN handler for enabling/disabling current checks

/*** ARM CANBus ***/
/******************/

CANInterface can(CANConfig::config);

// this function is indirectly triggered by an IRQ. It reads a CAN msg and puts in the mail_box
void rxCANPostman() {
  CANMsg msg;
  // this loop is needed to avoid missing msg received between turning off the IRQ and turning it back on
  while (can1.read(msg)) {
    // TODO: Handle mail related errors better
    CANMsg* mail = mail_box.try_alloc_for(1ms);
    MBED_ASSERT(mail != nullptr);
    *mail = msg;
    mail_box.put(mail);
  }
  can_irq_set(can1.getHandle(), IRQ_RX, true);
}

void rxCANISR() {
  can_irq_set(can1.getHandle(), IRQ_RX, false);
  event_queue.call(&rxCANPostman);
}

// Outgoing message processor
void txCANProcessor() {
  constexpr std::chrono::milliseconds txInterdelay = 2ms;
  constexpr std::chrono::milliseconds txPeriod     = 10ms;

  CANMsg txMsg;

  struct __attribute__((__packed__)) MotionReport {
    float position = 0, velocity = 0;
  };

  while (true) {
    MotionReport report;

    txMsg.setID(HWBRIDGE::CANID::REPORT_TURNTABLE_MOTION);
    report = {Turntable::manager.getActiveController()->reportAngleDeg(),
              Turntable::manager.getActiveController()->reportAngularVelocityDegPerSec()};
    txMsg.setPayload(report);
    can1.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_SHOULDER_MOTION);
    report = {Elbow::manager.getActiveController()->reportAngleDeg(),
              Elbow::manager.getActiveController()->reportAngularVelocityDegPerSec()};
    txMsg.setPayload(report);
    can1.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_ELBOW_MOTION);
    report = {Elbow::manager.getActiveController()->reportAngleDeg(),
              Elbow::manager.getActiveController()->reportAngularVelocityDegPerSec()};
    txMsg.setPayload(report);
    can1.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_LEFT_WRIST_MOTION);
    report = {Wrist::leftManager.getActiveController()->reportAngleDeg(),
              Wrist::leftManager.getActiveController()->reportAngularVelocityDegPerSec()};
    txMsg.setPayload(report);
    can1.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_RIGHT_WRIST_MOTION);
    report = {Wrist::rightManager.getActiveController()->reportAngleDeg(),
              Wrist::rightManager.getActiveController()->reportAngularVelocityDegPerSec()};
    txMsg.setPayload(report);
    can1.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    txMsg.setID(HWBRIDGE::CANID::REPORT_CLAW_MOTION);
    report = {Claw::manager.getActiveController()->reportAngleDeg(),
              Claw::manager.getActiveController()->reportAngularVelocityDegPerSec()};
    txMsg.setPayload(report);
    can1.write(txMsg);
    ThisThread::sleep_for(txInterdelay);

    ThisThread::sleep_for(txPeriod);
  }
}

Thread rxCANPostmanThread(osPriorityRealtime);
Thread rxCANClientThread(osPriorityAboveNormal);
Thread txCANProcessorThread(osPriorityBelowNormal);

int main() {
  HWBRIDGE::CANSignalValue_t setTurntablePosition;
  HWBRIDGE::CANSignalValue_t setShoulderPosition;
  HWBRIDGE::CANSignalValue_t setElbowPosition;
  HWBRIDGE::CANSignalValue_t setLeftWristPosition;
  HWBRIDGE::CANSignalValue_t setRightWristPosition;
  HWBRIDGE::CANSignalValue_t setClawPosition;
  HWBRIDGE::CANSignalValue_t setToolTipPosition;

  printf("\r\n\r\n");
  printf("ARM APPLICATION STARTED\r\n");
  printf("=======================\r\n");

  // CAN init stuff
  can1.setFilter(HWBRIDGE::CANFILTER::ROVER_CANID_FIRST_ARM_RX, CANStandard,
                 HWBRIDGE::ROVERCONFIG::ROVER_CANID_FILTER_MASK);

  rxCANPostmanThread.start(callback(&event_queue, &EventQueue::dispatch_forever));
  rxCANClientThread.start(&rxCANClient);
  txCANProcessorThread.start(txCANProcessor);

  can1.attach(&rxCANISR, CANBus::RxIrq);

  while (true) {
    // Process CAN RX signals (TODO: NEED TO HANDLE SNA CASES!!!)
    can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_TURNTABLE_SET_POSITION,
                           setTurntablePosition);
    can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SHOULDER_SET_POSITION,
                           setShoulderPosition);
    can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_ELBOW_SET_POSITION,
                           setElbowPosition);
    can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_LEFT_WRIST_SET_POSITION,
                           setLeftWristPosition);
    can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_RIGHT_WRIST_SET_POSITION,
                           setRightWristPosition);
    can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_CLAW_SET_POSITION,
                           setClawPosition);
    can.readStreamedSignal(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_TOOL_TIP_SET_POSITION,
                           setToolTipPosition);

    // Update joint set points
    Turntable::manager.getActiveController()->setSetPoint((float)setTurntablePosition));
    Shoulder::manager.getActiveController()->setSetPoint((float)setShoulderPosition);
    Elbow::manager.getActiveController()->setSetPoint((float)setElbowPosition);
    Wrist::leftManager.getActiveController()->setSetPoint((float)setLeftWristPosition);
    Wrist::rightManager.getActiveController()->setSetPoint((float)setRightWristPosition);
    Claw::manager.getActiveController()->setSetPoint((float)setClawPosition);
    Tooltip::clawTooltipServo.setValue((float)setToolTipPosition);

    // Compute actuator controls
    Turntable::manager.getActiveController()->update();
    Elbow::manager.getActiveController()->update();
    Shoulder::manager.getActiveController()->update();
    Wrist::leftManager.getActiveController()->update();
    Wrist::rightManager.getActiveController()->update();
    Claw::manager.getActiveController()->update();

    ThisThread::sleep_for(1ms);
  }
}

/*** ARM COMMAND HANDLER FUNCTIONS ***/
/*************************************/

static mbed_error_status_t armSetControlMode(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::ARM_SET_CONTROL_MODE) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  HWBRIDGE::CONTROL::MODE controlMode;
  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_arm_set_control_mode_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_arm_set_control_mode_unpack(&msgStruct, msgData.raw,
                                                      UWRT_MARS_ROVER_CAN_ARM_SET_CONTROL_MODE_LENGTH) == 0) {
    // Set joint control mode
    controlMode = (HWBRIDGE::CONTROL::MODE)uwrt_mars_rover_can_arm_set_control_mode_arm_joint_control_mode_decode(
        msgStruct.arm_joint_control_mode);
    success &= Turntable::manager.switchControlMode(controlMode);

    // Set shoulder control mode
    controlMode = (HWBRIDGE::CONTROL::MODE)uwrt_mars_rover_can_arm_set_control_mode_arm_shoulder_control_mode_decode(
        msgStruct.arm_shoulder_control_mode);
    success &= Shoulder::manager.switchControlMode(controlMode);

    // Set elbow control mode
    controlMode = (HWBRIDGE::CONTROL::MODE)uwrt_mars_rover_can_arm_set_control_mode_arm_elbow_control_mode_decode(
        msgStruct.arm_elbow_control_mode);
    success &= Elbow::manager.switchControlMode(controlMode);

    // Set left wrist control mode
    controlMode = (HWBRIDGE::CONTROL::MODE)uwrt_mars_rover_can_arm_set_control_mode_arm_left_wrist_control_mode_decode(
        msgStruct.arm_left_wrist_control_mode);
    success &= Wrist::leftManager.switchControlMode(controlMode);

    // Set right wrist control mode
    controlMode = (HWBRIDGE::CONTROL::MODE)uwrt_mars_rover_can_arm_set_control_mode_arm_right_wrist_control_mode_decode(
        msgStruct.arm_right_wrist_control_mode);
    success &= Wrist::rightManager.switchControlMode(controlMode);

    // Set claw control mode
    controlMode = (HWBRIDGE::CONTROL::MODE)uwrt_mars_rover_can_arm_set_control_mode_arm_claw_control_mode_decode(
        msgStruct.arm_claw_control_mode);
    success &= Claw::manager.switchControlMode(controlMode);
  } else {
    // Error unpacking!
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t armSetJointPIDParams(CANMsg& msg) {
  // Error check CAN ID
  if (msg.getID() != HWBRIDGE::CANID::ARM_SET_JOINT_PID_PARAMS) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }

  bool success = true;

  float value;
  HWBRIDGE::CANMsgData_t msgData;
  struct uwrt_mars_rover_can_arm_set_joint_pid_params_t msgStruct;

  // Unpack CAN data
  msg.getPayload(msgData);
  if (uwrt_mars_rover_can_arm_set_joint_pid_params_unpack(&msgStruct, msgData.raw,
                                                          UWRT_MARS_ROVER_CAN_ARM_SET_JOINT_PID_PARAMS_LENGTH) == 0) {
    // Determine joint ID
    HWBRIDGE::ARM_JOINT_PIDID_VALUES jointID =
        (HWBRIDGE::ARM_JOINT_PIDID_VALUES)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pidid_decode(
            msgStruct.arm_joint_pidid);

    const Utility::LookupTable<HWBRIDGE::ARM::ActuatorID, Controller::ActuatorControllerManager*> lut = {
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_TURNTABLE, &Turntable::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_SHOULDER & Shoulder::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_ELBOW, &Elbow::manager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_LEFT_WRIST, &Wrist::leftManager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_RIGHT_WRIST, &Wrist::rightManager},
        {HWBRIDGE::ARM_JOINT_PIDID_VALUES::ARM_JOINT_PIDID_CLAW, &Claw::manager}};

    auto act = lut.at(data.actuatorID).value_or(nullptr);
    if (!act) {
      return MBED_ERROR_INVALID_ARGUMENT;
    }

    // Extract PID params
    float p = (float)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pid_proportional_gain_decode(
        msgStruct.arm_joint_pid_proportional_gain);
    float i = (float)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pid_integral_gain_decode(
        msgStruct.arm_joint_pid_integral_gain);
    float d = (float)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pid_derivative_gain_decode(
        msgStruct.arm_joint_pid_derivative_gain);
    float deadzone = (float)uwrt_mars_rover_can_arm_set_joint_pid_params_arm_joint_pid_deadzone_decode(
        msgStruct.arm_joint_pid_proportional_gain);

    // Set PID params
    if (auto pid = act->getActiveController()->getPID()) {
      if (msgStruct.arm_joint_pid_proportional_gain !=
          HWBRIDGE::ARM_TURNTABLE_PID_PROPORTIONAL_GAIN_VALUES::ARM_TURNTABLE_PID_PROPORTIONAL_GAIN_SNA) {
        pid.value().get().updateProportionalGain(p);
      }
      if (msgStruct.arm_joint_pid_integral_gain !=
          HWBRIDGE::ARM_TURNTABLE_PID_INTEGRAL_GAIN_VALUES::ARM_TURNTABLE_PID_INTEGRAL_GAIN_SNA) {
        pid.value().get().updateIntegralGain(i);
      }
      if (msgStruct.arm_joint_pid_derivative_gain !=
          HWBRIDGE::ARM_TURNTABLE_PID_DERIVATIVE_GAIN_VALUES::ARM_TURNTABLE_PID_DERIVATIVE_GAIN_SNA) {
        pid.value().get().updateDerivativeGain(d);
      }
      if (msgStruct.arm_joint_pid_deadzone !=
          HWBRIDGE::ARM_TURNTABLE_PID_DEADZONE_VALUES::ARM_TURNTABLE_PID_DEADZONE_SNA) {
        pid.value().get().updateDeadzone(deadzone);
      }
    } else {
      // PID controller doesn't exist!
      success = false;
    }
  } else {
    // Error unpacking!
    success = false;
  }

  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}
