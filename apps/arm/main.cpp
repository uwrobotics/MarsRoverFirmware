#include "mbed.h"

#include "arm_config.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "ActuatorController.h"

Motor turnTableMotor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false);
Motor shoulderMotor(MTR_PWM_SHLDR, MTR_DIR_SHLDR, false);
Motor elbowMotor(MTR_PWM_ELBW, MTR_DIR_ELBW, false);
Motor wristLeftMotor(MTR_PWM_WRST_LHS, MTR_DIR_WRST_LHS, false);
Motor wristRightMotor(MTR_PWM_WRIST_RHS, MTR_DIR_WRIST_RHS, false);
Motor clawMotor(MTR_PWM_CLAW, MTR_DIR_CLAW, false);

EncoderAbsolute_PWM turnTableEncoder(turnTableEncoderConfig);
EncoderAbsolute_PWM shoulderEncoder(shoulderEncoderConfig);
EncoderAbsolute_PWM elbowEncoder(elbowEncoderConfig);

EnocderRelative_Quadrature wristLeftEncoder(wristLeftEncoderConfig);
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

int main()
{
    while (true) {
        led1 = !led1;
        wait_ms(k_interval_ms);
    }
}
