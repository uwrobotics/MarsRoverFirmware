#include "mbed.h"
#include "can_config.h"

#include "ArmConfig.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "ActuatorController.h"
#include "DifferentialWristController.h"
#include "ClawController.h"
#include "CANMsg.h"
#include "CANBuffer.h"

/*** ARM COMPONENTS ***/
/**********************/

// Motors
Motor turnTableMotor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false);
Motor shoulderMotor(MTR_PWM_SHLDR, MTR_DIR_SHLDR, false);
Motor elbowMotor(MTR_PWM_ELBW, MTR_DIR_ELBW, false);
Motor wristLeftMotor(MTR_PWM_WRST_LHS, MTR_DIR_WRST_LHS, false);
Motor wristRightMotor(MTR_PWM_WRST_RHS, MTR_DIR_WRST_RHS, false);
Motor clawMotor(MTR_PWM_CLAW, MTR_DIR_CLAW, false);

// Encoders
EncoderAbsolute_PWM turnTableEncoder(ArmConfig::turnTableEncoderConfig);
EncoderAbsolute_PWM shoulderEncoder(ArmConfig::shoulderEncoderConfig);
EncoderAbsolute_PWM elbowEncoder(ArmConfig::elbowEncoderConfig);
EncoderRelative_Quadrature wristLeftEncoder(ArmConfig::wristLeftEncoderConfig);
EncoderRelative_Quadrature wristRightEncoder(ArmConfig::wristRightEncoderConfig);
EncoderRelative_Quadrature clawEncoder(ArmConfig::clawEncoderConfig);

// Limit switches
DigitalIn turnTableLimLeft(LIM_TRNTBL_LHS);
DigitalIn turnTableLimRight(LIM_TRNTBL_RHS);
DigitalIn elbowLimUp(LIM_ELBW_UP);
DigitalIn elbowLimDown(LIM_ELBW_DN);
DigitalIn wristLimUp(LIM_WRST_UP);
DigitalIn wristLimDown(LIM_WRST_DN);
DigitalIn wristLimCenter(LIM_WRST_CNTR);
DigitalIn clawLimOpen(LIM_CLAW_OPEN);

// Analog inputs
AnalogIn clawForceSensor(FORCE_CLAW);

// Rotary actuators
ActuatorController turnTableActuator(ArmConfig::turnTableActuatorConfig, turnTableMotor, turnTableEncoder, turnTableLimRight, turnTableLimLeft);
ActuatorController shoulderActuator(ArmConfig::shoulderActuatorConfig, shoulderMotor, shoulderEncoder);
ActuatorController elbowActuator(ArmConfig::elbowActuatorConfig, elbowMotor, elbowEncoder, elbowLimDown, elbowLimUp);
ActuatorController wristLeftActuator(ArmConfig::wristLeftActuatorConfig, wristLeftMotor, wristLeftEncoder);
ActuatorController wristRightActuator(ArmConfig::wristRightActuatorConfig, wristRightMotor, wristRightEncoder);

// Complex controllers
DifferentialWristController wristController(wristLeftActuator, wristRightActuator, wristLimUp, wristLimCenter, wristLimDown);
ClawController clawController(ArmConfig::clawActuatorConfig, clawMotor, clawEncoder, clawLimOpen, clawForceSensor);

/*** ARM COMMAND HANDLER FUNCTIONS ***/
/*************************************/

// Set the control mode of a joint (motor power / velocity / position)
static mbed_error_status_t setControlMode(CANMsg &msg) {
    ActuatorController::t_actuatorControlMode controlMode;
    msg.getPayload(controlMode);

    switch(msg.id) {
        case CANID::SET_TURNTABLE_CONTROL_MODE:
            return turnTableActuator.setControlMode(controlMode);
        case CANID::SET_SHOULDER_CONTROL_MODE:
            return shoulderActuator.setControlMode(controlMode);
        case CANID::SET_ELBOW_CONTROL_MODE:
            return elbowActuator.setControlMode(controlMode);
        case CANID::SET_WRIST_CONTROL_MODE:
            return wristController.setControlMode(controlMode);
        case CANID::SET_CLAW_CONTROL_MODE:
            return clawController.setControlMode(controlMode);
        default:
            return MBED_ERROR_INVALID_ARGUMENT;
    }
}

// Set the motion data (motor power / velocity / position) of a joint
static mbed_error_status_t setMotionData(CANMsg &msg) {
    float motionData;
    msg.getPayload(motionData);

        switch(msg.id) {
        case CANID::SET_TURNTABLE_MOTIONDATA:
            return turnTableActuator.setMotionData(motionData);
        case CANID::SET_SHOULDER_MOTIONDATA:
            return shoulderActuator.setMotionData(motionData);
        case CANID::SET_ELBOW_MOTIONDATA:
            return elbowActuator.setMotionData(motionData);
        case CANID::SET_WRIST_PITCH_MOTIONDATA:
            return wristController.setPitchMotionData(motionData);
        case CANID::SET_WRIST_ROLL_MOTIONDATA:
            return wristController.setRollMotionData(motionData);
        case CANID::SET_CLAW_MOTIONDATA:
            return clawController.setMotionData(motionData);
        default:
            return MBED_ERROR_INVALID_ARGUMENT;
    }
}

// Run wrist calibration routine
static mbed_error_status_t runWristCalibration(CANMsg &msg) {
    bool runCalibration;
    msg.getPayload(runCalibration);

    if (runCalibration) {
        return wristController.runPositionCalibration();
    }

    return MBED_SUCCESS;
}

// Run claw calibration routine
static mbed_error_status_t runClawCalibration(CANMsg &msg) {
    bool runCalibration;
    msg.getPayload(runCalibration);

    if (runCalibration) {
        return clawController.runPositionCalibration();
    }

    return MBED_SUCCESS;
}

// Deploy or retract tool tip
static mbed_error_status_t setToolTipDeployment(CANMsg &msg) {
    return MBED_SUCCESS; // TODO
}

// Enable or disable PID tuning mode
static mbed_error_status_t setPIDTuningMode(CANMsg &msg) {
    return MBED_SUCCESS; // TODO
}

// Configure PID parameters
static mbed_error_status_t setPIDParameter(CANMsg &msg) {
    return MBED_SUCCESS; // TODO
}

// Handler function mappings
static CANMsg::CANMsgHandlerMap canHandlerMap = {
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

    {CANID::SET_PID_DEADZONE,           &setPIDParameter},
    {CANID::SET_JOINT_PID_P,            &setPIDParameter},
    {CANID::SET_JOINT_PID_I,            &setPIDParameter},
    {CANID::SET_JOINT_PID_D,            &setPIDParameter},
    {CANID::SET_JOINT_PID_BIAS,         &setPIDParameter}
};

/*** ARM CANBus ***/
/******************/

// Interface and recieve buffer
CAN can1(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);
CANBuffer rxCANBuffer(can1, CANBuffer::BufferType::rx);

// Incoming message processor
void rxCANProcessor() {
    CANMsg rxMsg;

    while (true) {
        rxCANBuffer.waitFlagsAny(CANBUFFER_FLAG_DATA_READY);

        if (rxCANBuffer.pop(rxMsg) && (canHandlerMap.find(rxMsg.id) != canHandlerMap.end())) {
            canHandlerMap[rxMsg.id](rxMsg);
        }
        
        ThisThread::sleep_for(20);
    }
}

// Outgoing message processor
void txCANProcessor() {
    CANMsg txMsg;

    while (true) {
        txMsg.id = TURNTABLE_POSITION;
        txMsg.setPayload(turnTableActuator.getAngle_Degrees());
        can1.write(txMsg);
        ThisThread::sleep_for(200);

        txMsg.id = SHOULDER_POSITION;
        txMsg.setPayload(shoulderActuator.getAngle_Degrees());
        can1.write(txMsg);
        ThisThread::sleep_for(20);

        txMsg.id = ELBOW_POSITION;
        txMsg.setPayload(elbowActuator.getAngle_Degrees());
        can1.write(txMsg);
        ThisThread::sleep_for(200);

        txMsg.id = WRIST_PITCH_POSITION;
        txMsg.setPayload(wristController.getPitchAngle_Degrees());
        can1.write(txMsg);
        ThisThread::sleep_for(200);

        txMsg.id = WRIST_ROLL_POSITION;
        txMsg.setPayload(wristController.getRollAngle_Degrees());
        can1.write(txMsg);
        ThisThread::sleep_for(200);

        txMsg.id = CLAW_POSITION;
        txMsg.setPayload(clawController.getGapDistance_Cm());
        can1.write(txMsg);
        ThisThread::sleep_for(200);
    }
}

Thread rxCANProcessorThread;
Thread txCANProcessorThread;

DigitalOut led1(LED1);

int main()
{

    rxCANProcessorThread.start(rxCANProcessor);
    txCANProcessorThread.start(txCANProcessor);

    while (true) {
        led1 = !led1;
        ThisThread::sleep_for(500);
    }
}
