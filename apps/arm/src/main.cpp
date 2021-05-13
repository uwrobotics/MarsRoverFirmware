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

/*** ARM COMMAND HANDLER FUNCTIONS ***/
/*************************************/

static mbed_error_status_t setControlMode(CANMsg &msg) {
  HWBRIDGE::CONTROL::Mode controlMode;
  msg.getPayload(controlMode);
  bool success;

  switch (msg.getID()) {
    case HWBRIDGE::CANID::SET_TURNTABLE_CONTROL_MODE:
      success = Turntable::manager.switchControlMode(controlMode);
      break;
    case HWBRIDGE::CANID::SET_SHOULDER_CONTROL_MODE:
      success = Shoulder::manager.switchControlMode(controlMode);
      break;
    case HWBRIDGE::CANID::SET_ELBOW_CONTROL_MODE:
      success = Elbow::manager.switchControlMode(controlMode);
      break;
    case HWBRIDGE::CANID::SET_LEFT_WRIST_CONTROL_MODE:
      success = Wrist::leftManager.switchControlMode(controlMode);
      break;
    case HWBRIDGE::CANID::SET_RIGHT_WRIST_CONTROL_MODE:
      success = Wrist::rightManager.switchControlMode(controlMode);
      break;
    case HWBRIDGE::CANID::SET_CLAW_CONTROL_MODE:
      success = Claw::manager.switchControlMode(controlMode);
      break;
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

static mbed_error_status_t setMotionData(CANMsg &msg) {
  float motionData;
  msg.getPayload(motionData);

  switch (msg.getID()) {
    case HWBRIDGE::CANID::SET_TURNTABLE_MOTIONDATA:
      Turntable::manager.getActiveController()->setSetPoint(motionData);
      break;
    case HWBRIDGE::CANID::SET_SHOULDER_MOTIONDATA:
      Shoulder::manager.getActiveController()->setSetPoint(motionData);
      break;
    case HWBRIDGE::CANID::SET_ELBOW_MOTIONDATA:
      Elbow::manager.getActiveController()->setSetPoint(motionData);
      break;
    case HWBRIDGE::CANID::SET_LEFT_WRIST_MOTIONDATA:
      Wrist::leftManager.getActiveController()->setSetPoint(motionData);
      break;
    case HWBRIDGE::CANID::SET_RIGHT_WRIST_MOTIONDATA:
      Wrist::rightManager.getActiveController()->setSetPoint(motionData);
      break;
    case HWBRIDGE::CANID::SET_CLAW_MOTIONDATA:
      Claw::manager.getActiveController()->setSetPoint(motionData);
      break;
    case HWBRIDGE::CANID::SET_TOOL_TIP_MOTIONDATA:
      Tooltip::clawTooltipServo.setValue(motionData);
      break;
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
  return MBED_SUCCESS;
}

static mbed_error_status_t setSafetyCheck(CANMsg &msg) {
  HWBRIDGE::CONTROL::SAFETY::SafetyCheckPayload data;
  msg.getPayload(data);

  const Utility::LookupTable<HWBRIDGE::ARM::ActuatorID, Controller::ActuatorControllerManager *> lut = {
      {HWBRIDGE::ARM::ActuatorID::TURNTABLE, &Turntable::manager},
      {HWBRIDGE::ARM::ActuatorID::SHOULDER, &Shoulder::manager},
      {HWBRIDGE::ARM::ActuatorID::ELBOW, &Elbow::manager},
      {HWBRIDGE::ARM::ActuatorID::WRISTLEFT, &Wrist::leftManager},
      {HWBRIDGE::ARM::ActuatorID::WRISTRIGHT, &Wrist::rightManager},
      {HWBRIDGE::ARM::ActuatorID::CLAW, &Claw::manager}};

  auto act = lut.at(data.actuatorID).value_or(nullptr);
  if (!act) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }
  auto temp = act->getActiveController();

  switch (msg.getID()) {
    case HWBRIDGE::CANID::SET_JOINT_CURRENT_CHECK:
      data.check ? temp->activateCurrentChecks() : temp->deactivateCurrentChecks();
      break;

    case HWBRIDGE::CANID::SET_JOINT_DEG_PER_SEC_CHECK:
      data.check ? temp->activateDegPerSecChecks() : temp->deactivateDegPerSecChecks();
      break;

    case HWBRIDGE::CANID::SET_JOINT_LIMIT_SWITCH:
      data.check ? temp->activateLimitSwitchChecks() : temp->deactivateLimitSwitchChecks();
      break;

    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }

  return MBED_SUCCESS;
}

static mbed_error_status_t setPIDParameter(CANMsg &msg) {
  HWBRIDGE::CONTROL::PID::TuningApiPayload data;
  msg.getPayload(data);

  const Utility::LookupTable<HWBRIDGE::ARM::ActuatorID, Controller::ActuatorControllerManager *> lut = {
      {HWBRIDGE::ARM::ActuatorID::TURNTABLE, &Turntable::manager},
      {HWBRIDGE::ARM::ActuatorID::SHOULDER, &Shoulder::manager},
      {HWBRIDGE::ARM::ActuatorID::ELBOW, &Elbow::manager},
      {HWBRIDGE::ARM::ActuatorID::WRISTLEFT, &Wrist::leftManager},
      {HWBRIDGE::ARM::ActuatorID::WRISTRIGHT, &Wrist::rightManager},
      {HWBRIDGE::ARM::ActuatorID::CLAW, &Claw::manager}};

  auto act = lut.at(data.actuatorID).value_or(nullptr);
  if (!act) {
    return MBED_ERROR_INVALID_ARGUMENT;
  }
  bool success;

  switch (msg.getID()) {
    case HWBRIDGE::CANID::SET_JOINT_PID_DEADZONE:
      if (auto temp = act->getActiveController()->getPID()) {
        temp.value().get().updateDeadzone(data.value);
        success = true;
      } else {
        success = false;
      }
      break;
    case HWBRIDGE::CANID::SET_JOINT_PID_P:
      if (auto temp = act->getActiveController()->getPID()) {
        temp.value().get().updateProportionalGain(data.value);
        success = true;
      } else {
        success = false;
      }
      break;
    case HWBRIDGE::CANID::SET_JOINT_PID_I:
      if (auto temp = act->getActiveController()->getPID()) {
        temp.value().get().updateIntegralGain(data.value);
        success = true;
      } else {
        success = false;
      }
      break;
    case HWBRIDGE::CANID::SET_JOINT_PID_D:
      if (auto temp = act->getActiveController()->getPID()) {
        temp.value().get().updateDerivativeGain(data.value);
        success = true;
      } else {
        success = false;
      }
      break;
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
  return success ? MBED_SUCCESS : MBED_ERROR_CODE_FAILED_OPERATION;
}

// Handler function mappings
const static CANMsg::CANMsgHandlerMap canHandlerMap = {{HWBRIDGE::CANID::SET_TURNTABLE_CONTROL_MODE, &setControlMode},
                                                       {HWBRIDGE::CANID::SET_SHOULDER_CONTROL_MODE, &setControlMode},
                                                       {HWBRIDGE::CANID::SET_ELBOW_CONTROL_MODE, &setControlMode},
                                                       {HWBRIDGE::CANID::SET_LEFT_WRIST_CONTROL_MODE, &setControlMode},
                                                       {HWBRIDGE::CANID::SET_RIGHT_WRIST_CONTROL_MODE, &setControlMode},
                                                       {HWBRIDGE::CANID::SET_CLAW_CONTROL_MODE, &setControlMode},

                                                       {HWBRIDGE::CANID::SET_TURNTABLE_MOTIONDATA, &setMotionData},
                                                       {HWBRIDGE::CANID::SET_SHOULDER_MOTIONDATA, &setMotionData},
                                                       {HWBRIDGE::CANID::SET_ELBOW_MOTIONDATA, &setMotionData},
                                                       {HWBRIDGE::CANID::SET_LEFT_WRIST_MOTIONDATA, &setMotionData},
                                                       {HWBRIDGE::CANID::SET_RIGHT_WRIST_MOTIONDATA, &setMotionData},
                                                       {HWBRIDGE::CANID::SET_CLAW_MOTIONDATA, &setMotionData},
                                                       {HWBRIDGE::CANID::SET_TOOL_TIP_MOTIONDATA, &setMotionData},

                                                       {HWBRIDGE::CANID::SET_JOINT_CURRENT_CHECK, &setSafetyCheck},
                                                       {HWBRIDGE::CANID::SET_JOINT_LIMIT_SWITCH, &setSafetyCheck},
                                                       {HWBRIDGE::CANID::SET_JOINT_DEG_PER_SEC_CHECK, &setSafetyCheck},

                                                       {HWBRIDGE::CANID::SET_JOINT_PID_DEADZONE, &setPIDParameter},
                                                       {HWBRIDGE::CANID::SET_JOINT_PID_P, &setPIDParameter},
                                                       {HWBRIDGE::CANID::SET_JOINT_PID_I, &setPIDParameter},
                                                       {HWBRIDGE::CANID::SET_JOINT_PID_D, &setPIDParameter}};

/*** ARM CANBus ***/
/******************/

CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

// CAN Processing Objects
Mail<CANMsg, 100> mail_box;
EventQueue event_queue;

void rxCANClient() {
  while (true) {
    CANMsg *mail = nullptr;
    do {
      mail = mail_box.try_get();  // TODO: try_get_for was not working. Investigate why and use it
      ThisThread::sleep_for(1ms);
    } while (mail == nullptr);
    MBED_ASSERT((mail != nullptr));
    canHandlerMap.at(mail->getID())(*mail);
    MBED_ASSERT(mail_box.free(mail) == osOK);
  }
}

// this function is indirectly triggered by an IRQ. It reads a CAN msg and puts in the mail_box
void rxCANPostman() {
  CANMsg msg;
  // this loop is needed to avoid missing msg received between turning off the IRQ and turning it back on
  while (can1.read(msg)) {
    // TODO: Handle mail related errors better
    CANMsg *mail = mail_box.try_alloc_for(1ms);
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
