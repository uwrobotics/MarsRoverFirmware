#include "mbed.h"

#include "arm_config.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "ActuatorController.h"
#include "DifferentialWristController.h"
#include "CANMsg.h"
#include <map>

#include "can_config.h"

Motor turnTableMotor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false);
Motor shoulderMotor(MTR_PWM_SHLDR, MTR_DIR_SHLDR, false);
Motor elbowMotor(MTR_PWM_ELBW, MTR_DIR_ELBW, false);
Motor wristLeftMotor(MTR_PWM_WRST_LHS, MTR_DIR_WRST_LHS, false);
Motor wristRightMotor(MTR_PWM_WRST_RHS, MTR_DIR_WRST_RHS, false);
Motor clawMotor(MTR_PWM_CLAW, MTR_DIR_CLAW, false);

EncoderAbsolute_PWM turnTableEncoder(turnTableEncoderConfig);
EncoderAbsolute_PWM shoulderEncoder(shoulderEncoderConfig);
EncoderAbsolute_PWM elbowEncoder(elbowEncoderConfig);

EncoderRelative_Quadrature wristLeftEncoder(wristLeftEncoderConfig);
EncoderRelative_Quadrature wristRightEncoder(wristRightEncoderConfig);
EncoderRelative_Quadrature clawEncoder(clawEncoderConfig);

DigitalIn turnTableLimLeft(LIM_TRNTBL_LHS);
DigitalIn turnTableLimRight(LIM_TRNTBL_RHS);
DigitalIn elbowLimUp(LIM_ELBW_UP);
DigitalIn elbowLimDown(LIM_ELBW_DN);
DigitalIn wristLimUp(LIM_WRST_UP);
DigitalIn wristLimDown(LIM_WRST_DN);
DigitalIn wristLimCenter(LIM_WRST_CNTR);
DigitalIn clawLimOpen(LIM_CLAW_OPEN);

ActuatorController turnTableActuator(turnTableActuatorConfig, turnTableMotor, turnTableEncoder, turnTableLimRight, turnTableLimLeft);
ActuatorController shoulderActuator(shoulderActuatorConfig, shoulderMotor, shoulderEncoder);
ActuatorController elbowActuator(elbowActuatorConfig, elbowMotor, elbowEncoder, elbowLimDown, elbowLimUp);

ActuatorController wristLeftActuator(wristLeftActuatorConfig, wristLeftMotor, wristLeftEncoder);
ActuatorController wristRightctuator(wristRightActuatorConfig, wristRightMotor, wristRightEncoder);

DifferentialWristController wristController(wristLeftActuator, wristRightctuator, wristLimUp, wristLimCenter, wristLimDown);

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
            // TODO
            break;
    }

    return MBED_SUCCESS;
}

std::map<CANCommandIDs, CANMsg::CANMsgHandler> canIDHandlerMap;




int main()
{
    canIDHandlerMap[SET_TURNTABLE_CONTROL_MODE] = &setControlMode;

    while (true) {
        led1 = !led1;
        wait_ms(500);
    }
}
