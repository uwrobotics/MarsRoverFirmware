#include "mbed.h"

#include "ArmConfig.h"
#include "AnalogIn.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "ActuatorController.h"
#include "DifferentialWristController.h"
#include "ClawController.h"
#include "CANMsg.h"
#include "CANBuffer.h"
#include <map>

#include "can_config.h"

Motor turnTableMotor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false);
Motor shoulderMotor(MTR_PWM_SHLDR, MTR_DIR_SHLDR, false);
Motor elbowMotor(MTR_PWM_ELBW, MTR_DIR_ELBW, false);
Motor wristLeftMotor(MTR_PWM_WRST_LHS, MTR_DIR_WRST_LHS, false);
Motor wristRightMotor(MTR_PWM_WRST_RHS, MTR_DIR_WRST_RHS, false);
Motor clawMotor(MTR_PWM_CLAW, MTR_DIR_CLAW, false);

EncoderAbsolute_PWM turnTableEncoder(ArmConfig::turnTableEncoderConfig);
EncoderAbsolute_PWM shoulderEncoder(ArmConfig::shoulderEncoderConfig);
EncoderAbsolute_PWM elbowEncoder(ArmConfig::elbowEncoderConfig);

EncoderRelative_Quadrature wristLeftEncoder(ArmConfig::wristLeftEncoderConfig);
EncoderRelative_Quadrature wristRightEncoder(ArmConfig::wristRightEncoderConfig);
EncoderRelative_Quadrature clawEncoder(ArmConfig::clawEncoderConfig);

DigitalIn turnTableLimLeft(LIM_TRNTBL_LHS);
DigitalIn turnTableLimRight(LIM_TRNTBL_RHS);
DigitalIn elbowLimUp(LIM_ELBW_UP);
DigitalIn elbowLimDown(LIM_ELBW_DN);
DigitalIn wristLimUp(LIM_WRST_UP);
DigitalIn wristLimDown(LIM_WRST_DN);
DigitalIn wristLimCenter(LIM_WRST_CNTR);
DigitalIn clawLimOpen(LIM_CLAW_OPEN);

AnalogIn clawForceSensor(FORCE_CLAW);

ActuatorController turnTableActuator(ArmConfig::turnTableActuatorConfig, turnTableMotor, turnTableEncoder, turnTableLimRight, turnTableLimLeft);
ActuatorController shoulderActuator(ArmConfig::shoulderActuatorConfig, shoulderMotor, shoulderEncoder);
ActuatorController elbowActuator(ArmConfig::elbowActuatorConfig, elbowMotor, elbowEncoder, elbowLimDown, elbowLimUp);

ActuatorController wristLeftActuator(ArmConfig::wristLeftActuatorConfig, wristLeftMotor, wristLeftEncoder);
ActuatorController wristRightActuator(ArmConfig::wristRightActuatorConfig, wristRightMotor, wristRightEncoder);

DifferentialWristController wristController(wristLeftActuator, wristRightActuator, wristLimUp, wristLimCenter, wristLimDown);
ClawController clawController(ArmConfig::clawActuatorConfig, clawMotor, clawEncoder, clawLimOpen, clawForceSensor);

DigitalOut led1(LED1);

static mbed_error_status_t setControlMode(CANMsg &msg) {
    ActuatorController::t_actuatorControlMode controlMode;
    msg.getPayload(controlMode);

    switch(msg.id) {
        case SET_TURNTABLE_CONTROL_MODE:
            return turnTableActuator.setControlMode(controlMode);
        case SET_SHOULDER_CONTROL_MODE:
            return shoulderActuator.setControlMode(controlMode);
        case SET_ELBOW_CONTROL_MODE:
            return elbowActuator.setControlMode(controlMode);
        case SET_WRIST_CONTROL_MODE:
            return wristController.setControlMode(controlMode);
        case SET_CLAW_CONTROL_MODE:
            return clawController.setControlMode(controlMode);
        default:
            return MBED_ERROR_INVALID_ARGUMENT;
    }
}

static mbed_error_status_t setMotionData(CANMsg &msg) {
    float motionData;
    msg.getPayload(motionData);

        switch(msg.id) {
        case SET_TURNTABLE_MOTIONDATA:
            return turnTableActuator.setMotionData(motionData);
        case SET_SHOULDER_MOTIONDATA:
            return shoulderActuator.setMotionData(motionData);
        case SET_ELBOW_MOTIONDATA:
            return elbowActuator.setMotionData(motionData);
        case SET_WRIST_PITCH_MOTIONDATA:
            return wristController.setPitchMotionData(motionData);
        case SET_WRIST_ROLL_MOTIONDATA:
            return wristController.setRollMotionData(motionData);
        case SET_CLAW_MOTIONDATA:
            return clawController.setMotionData(motionData);
        default:
            return MBED_ERROR_INVALID_ARGUMENT;
    }
}

static mbed_error_status_t runWristCalibration(CANMsg &msg) {
    bool runCalibration;
    msg.getPayload(runCalibration);

    if (runCalibration) {
        return wristController.runPositionCalibration();
    }

    return MBED_SUCCESS;
}

static mbed_error_status_t runClawCalibration(CANMsg &msg) {
    bool runCalibration;
    msg.getPayload(runCalibration);

    if (runCalibration) {
        return clawController.runPositionCalibration();
    }

    return MBED_SUCCESS;
}

static mbed_error_status_t setToolTipDeployment(CANMsg &msg) {
    return MBED_SUCCESS; // TODO
}

static mbed_error_status_t setPIDTuningMode(CANMsg &msg) {
    return MBED_SUCCESS; // TODO
}

static mbed_error_status_t setPIDConstant(CANMsg &msg) {
    return MBED_SUCCESS; // TODO
}

static std::map<unsigned int, CANMsg::CANMsgHandler> canIDHandlerMap = {
    {CANID::SET_TURNTABLE_CONTROL_MODE, &setControlMode},
    {CANID::SET_SHOULDER_CONTROL_MODE,  &setControlMode},
    {CANID::SET_ELBOW_CONTROL_MODE,     &setControlMode},
    {CANID::SET_WRIST_CONTROL_MODE,     &setControlMode},
    {CANID::SET_CLAW_CONTROL_MODE,      &setControlMode},

    {CANID::SET_TURNTABLE_MOTIONDATA,   &setMotionData},
    {CANID::SET_SHOULDER_MOTIONDATA,    &setMotionData},
    {CANID::SET_ELBOW_MOTIONDATA,       &setMotionData},
    {CANID::SET_WRIST_PITCH_MOTIONDATA, &setMotionData},
    {CANID::SET_WRIST_ROLL_MOTIONDATA,  &setMotionData},
    {CANID::SET_CLAW_MOTIONDATA,        &setMotionData},
    {CANID::SET_TOOL_TIP_DEPLOYMENT,    &setToolTipDeployment},

    {CANID::RUN_WRIST_CALIBRATION,      &runWristCalibration},
    {CANID::RUN_CLAW_CALIBRATION,       &runClawCalibration},

    {CANID::SET_PID_TUNING_MODE,        &setPIDTuningMode},

    {CANID::SET_PID_DEADZONE,           &setPIDConstant},
    {CANID::SET_JOINT_PID_P,            &setPIDConstant},
    {CANID::SET_JOINT_PID_I,            &setPIDConstant},
    {CANID::SET_JOINT_PID_D,            &setPIDConstant},
    {CANID::SET_JOINT_PID_BIAS,         &setPIDConstant}
};

CAN can1(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);
CANBuffer canBuffer(can1, CANBuffer::BufferType::rx);

void canProcessorThread() {
    CANMsg rxMsg;

    while (true) {
        canBuffer.waitFlagsAny(CANBUFFER_FLAG_DATA_READY);

        if (canBuffer.pop(rxMsg) && (canIDHandlerMap.find(rxMsg.id) != canIDHandlerMap.end())) {
            canIDHandlerMap[rxMsg.id](rxMsg);
        }
        
        ThisThread::sleep_for(100);
    }
}

Thread canThread;

int main()
{

    canThread.start(canProcessorThread);

    while (true) {
        led1 = !led1;
        ThisThread::sleep_for(500);
    }
}
