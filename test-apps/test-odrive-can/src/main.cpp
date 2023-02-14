#include "CANConfig.h"
#include "CANInterface.h"
#include "CANMsg.h"
//#include "uwrt_mars_rover_can_enums.h"
#include "hw_bridge.h"
#include "stdio.h"
#include "Logger.h"

/* TODO: create test driver */

// Check inital values through Odrive tool and then set them back

CANInterface can(CANConfig::config);

/* int print_double_as_bytes(HWBRIDGE::CANSignalValue_t val) {
    uint8_t * pointer = val;

    for (int i = 0; i < 8; i++) {
        printf("%X ", *pointer + i);
    }
    printf("\n");*/
//}

// main() runs in its own thread in the OS
int main() {

    Utility::logger << "CAN Test : Axis 0\n";  // Band-aid fix for logger bug (issue #328)
    
    printf("CAN test: Axis 0\r\n");
    
    can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

    HWBRIDGE::CANSignalValue_t heartbeat = 0;
    
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_ERROR_AXIS0,
                         heartbeat);
    printf("TX test signal 2 value: %u\r\n", static_cast<uint16_t>(heartbeat));

    ThisThread::sleep_for(1000ms);

    HWBRIDGE::CANSignalValue_t inputPos = 0;
    HWBRIDGE::CANSignalValue_t closedLoop = 8;

    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_POS_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_POS_AXIS0,
                         inputPos);
    printf("set input pos to 0 \n");                   
    ThisThread::sleep_for(1000ms);

    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_REQUESTED_STATE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_REQUESTED_STATE_AXIS0,
                         closedLoop);
    printf("set closed loop control \n");                   
    ThisThread::sleep_for(1000ms);

    inputPos = 4;
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_POS_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_POS_AXIS0,
                         inputPos);
    printf("set input pos to 4 \n");                   
    ThisThread::sleep_for(1000ms);

    
    
    /*HWBRIDGE::CANSignalValue_t heartbeatAxisError;
    HWBRIDGE::CANSignalValue_t heartbeatCurrentState;
    HWBRIDGE::CANSignalValue_t heartbeatControllerStatus;

    HWBRIDGE::CANSignalValue_t motorError;
    HWBRIDGE::CANSignalValue_t encoderError;
    HWBRIDGE::CANSignalValue_t sensorlessError;

    HWBRIDGE::CANSignalValue_t nodeIDresponse;
    HWBRIDGE::CANSignalValue_t requestedStateResponse;

    HWBRIDGE::CANSignalValue_t encoderPosEstimate;
    HWBRIDGE::CANSignalValue_t encoderVelEstimate;

    HWBRIDGE::CANSignalValue_t encoderShadowCount;
    HWBRIDGE::CANSignalValue_t encoderCountinCPR;

    HWBRIDGE::CANSignalValue_t IqSetpoint;
    HWBRIDGE::CANSignalValue_t IqMeasured;

    HWBRIDGE::CANSignalValue_t sensorlessPosEstimate;
    HWBRIDGE::CANSignalValue_t sensorlessVelEstimate;

    HWBRIDGE::CANSignalValue_t vbusVoltage;

    // https://discourse.odriverobotics.com/t/can-interface-available-for-testing/1448
    // https://docs.odriverobotics.com/v/latest/fibre_types/com_odriverobotics_ODrive.html#ODrive.Controller.ControlMode
    // https://docs.odriverobotics.com/v/latest/can-guide.html#can-guide

    // Set CAN filters
    can.setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK);

    // Axis 0
    printf("Axis 0 :\n");
    
    // Get heartbeat messages
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_ERROR_AXIS0,
                         heartbeatAxisError);
    printf("heartbeatAxisError : %d\n", heartbeatAxisError);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_CURRENT_STATE_AXIS0,
                         heartbeatCurrentState);
    printf("heartbeatCurrentState : %d\n", heartbeatCurrentState);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_CONTROLLER_STATUS_AXIS0,
                         heartbeatControllerStatus);
    printf("heartbeatControllerStatus : %d\n", heartbeatControllerStatus);

    // Get motor error
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_MOTOR_ERROR_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_MOTOR_ERROR_AXIS0,
                         motorError);
    printf("Motor Error : %d\n", motorError);
    if (motorError){
        //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_CLEAR_ERRORS_AXIS0, 1);
        printf("Error cleared. \n");
    }
    // Get encoder error
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_ERROR_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_ERROR_AXIS0,
                         encoderError);
    printf("Encoder Error : %d\n", motorError);
    if (encoderError){
        //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_CLEAR_ERRORS_AXIS0, 1);
        printf("Error cleared. \n");
    }
    // Get sensorless error
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_SENSORLESS_ERROR_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_SENSORLESS_ERROR_AXIS0,
                         sensorlessError);
    printf("sensorless Error : %d\n", motorError);
    if (sensorlessError){
        //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_CLEAR_ERRORS_AXIS0, 1);
        printf("Error cleared. \n");
    }

    // Set Node ID -> can be value from 0-63 or 0 to 16777215 (extended CAN)
    // might not need to get response
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_NODE_ID_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_CAN_NODE_ID_AXIS0, 1);
    printf("Node ID set to : %d\n", 1);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_NODE_ID_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_CAN_NODE_ID_AXIS0,
                         nodeIDresponse);
    
    // Set state
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_REQUESTED_STATE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_REQUESTED_STATE_AXIS0,
                         8);
    printf("Axis Requested State set to : %d\n", HWBRIDGE::ODRIVE_AXIS_CURRENT_STATE_AXIS0_VALUES::AXIS_STATE_CLOSED_LOOP_CONTROL);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_REQUESTED_STATE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_REQUESTED_STATE_AXIS0,
                         requestedStateResponse);
    printf("Requested State Response: ");
    //print_double_as_bytes(requestedStateResponse);
    

    // Get Encoder Estimates
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_ESTIMATES_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_POS_ESTIMATE_AXIS0,
                         encoderPosEstimate);
    printf("Encoder Position Estimate : %f\n");
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_ESTIMATES_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_VEL_ESTIMATE_AXIS0,
                         encoderVelEstimate);
    printf("Encoder Velocity Estimate : %f\n");

    // Get Encoder Count
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_COUNT_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_SHADOW_COUNT_AXIS0,
                         encoderShadowCount);
    printf("Encoder Shadow Count : %d\n");
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_COUNT_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_COUNTIN_CPR_AXIS0,
                         encoderCountinCPR); 
    printf("Encoder Count in CPR: %d\n");   

    // Set Controller Modes
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_CONTROLLER_MODES_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_CONTROL_MODE_AXIS0,
                         3);
    printf("Set Controller Mode to : %d\n", HWBRIDGE::ODRIVE_CONTROL_MODE_AXIS0_VALUES::CONTROL_MODE_POSITION_CONTROL);
    // Get response and confirm it matches
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_CONTROLLER_MODES_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_MODE_AXIS0,
                         0);
    printf("Set Controller Input Mode to: %d\n", 0);

    // Set Input Pos
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_POS_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_POS_AXIS0,
                         1.524);
    printf("Set Input Position to : %f\n", 1.524);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_POS_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_VEL_FF_AXIS0,
                         1);
    printf("Set Input Velocity FF to : %d\n", 1);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_POS_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_TORQUE_FF_POS_AXIS0,
                         1);
    printf("Set Input Torque FF to : %d\n", 1);

    // Set Input Vel
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_VEL_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_VEL_AXIS0,
                         1.524);
    printf("Set Input Velocity to : %f\n", 1.524);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_VEL_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_TORQUE_FF_AXIS0,
                         1.524);
    printf("Set Input Torque FF to : %f\n", 1.524);

    // Set Input Torque
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_TORQUE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_TORQUE_AXIS0,
                         1.524);
    // Set Limits
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_LIMITS_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_VELOCITY_LIMIT_AXIS0,
                         1.524);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_LIMITS_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_CURRENT_LIMIT_AXIS0,
                         1.524);
    // Set Anticogging
    //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_START_ANTICOGGING_AXIS0, 1);

    // Set Traj Vel Limit
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_TRAJ_VEL_LIMIT_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_TRAJ_VEL_LIMIT_AXIS0,
                         1.524);

    // Set Traj Accel Limits
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_TRAJ_ACCEL_LIMIT_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_TRAJ_ACCEL_LIMIT_AXIS0,
                         1.524);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_TRAJ_ACCEL_LIMIT_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_TRAJ_DECEL_LIMIT_AXIS0,
                         1.524);
    // Set Traj Inertia
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_TRAJ_INERTIA_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_TRAJ_INERTIA_AXIS0,
                         1.524);
    
    // Get IQ 
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_IQ_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_IQ_SETPOINT_AXIS0,
                         IqSetpoint);
    printf("IQ Setpoint : %f\n", IqSetpoint);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_IQ_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_IQ_MEASURED_AXIS0,
                         IqMeasured);
    printf("IQ Measured : %f\n", IqMeasured);
    // Get Sensorless Estimates
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_SENSORLESS_ESTIMATES_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_SENSORLESS_POS_ESTIMATE_AXIS0,
                         sensorlessPosEstimate);
    printf("Sensorless Position Estimate : %f\n", sensorlessPosEstimate);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_SENSORLESS_ESTIMATES_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_SENSORLESS_VEL_ESTIMATE_AXIS0,
                         sensorlessVelEstimate);
    // Get Vbus Voltage
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_VBUS_VOLTAGE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_VBUS_VOLTAGE_AXIS0,
                         vbusVoltage);
    printf("Vbus Voltage : %f\n", vbusVoltage);

    // clear errors
    //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_CLEAR_ERRORS_AXIS0, 1);

    // Set Linear Count
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_LINEAR_COUNT_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_POSITION_AXIS0,
                         0);
    // Set Position Gain
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_POSITION_GAIN_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_POS_GAIN_AXIS0,
                         2.451);
    // Set Vel Gains
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_VEL_GAIN_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_VEL_GAIN_AXIS0,
                         2.451);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_VEL_GAIN_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_VEL_INTEGRATOR_GAIN_AXIS0,
                         2.451);
    
    // Reboot Odrive
    //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_REBOOT_ODRIVE_AXIS0, 1);

    // wait until Odrive Responds again
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS0, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_CURRENT_STATE_AXIS0,
                         heartbeatCurrentState);

    // AXIS 1

    printf("Axis 1:\n");
    
    // Get heartbeat messages
    
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_ERROR_AXIS1,
                         heartbeatAxisError);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_CURRENT_STATE_AXIS1,
                         heartbeatCurrentState);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_CONTROLLER_STATUS_AXIS1,
                         heartbeatControllerStatus);

    // Get motor error
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_MOTOR_ERROR_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_MOTOR_ERROR_AXIS1,
                         motorError);
    //if (motorError){
        //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_CLEAR_ERRORS_AXIS1, 1);
    //}
    // Get encoder error
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_ERROR_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_ERROR_AXIS1,
                         encoderError);
    //if (encoderError){
        //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_CLEAR_ERRORS_AXIS1, 1);
    //}
    // Get sensorless error
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_SENSORLESS_ERROR_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_SENSORLESS_ERROR_AXIS1,
                         sensorlessError);
   // if (sensorlessError){
        //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_CLEAR_ERRORS_AXIS1, 1);
    //}

    printf("");

    // Set Node ID
    // might not need to get response
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_NODE_ID_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_CAN_NODE_ID_AXIS1, 1);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_NODE_ID_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_CAN_NODE_ID_AXIS1,
                         nodeIDresponse);
    
    // Set state
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_REQUESTED_STATE_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_REQUESTED_STATE_AXIS1,
                         8);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_AXIS_REQUESTED_STATE_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_REQUESTED_STATE_AXIS1,
                         requestedStateResponse);

    // Get Encoder Estimates
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_ESTIMATES_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_POS_ESTIMATE_AXIS1,
                         encoderPosEstimate);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_ESTIMATES_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_VEL_ESTIMATE_AXIS1,
                         encoderVelEstimate);

    // Get Encoder Count
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_COUNT_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_SHADOW_COUNT_AXIS1,
                         encoderShadowCount);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_ENCODER_COUNT_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_ENCODER_COUNTIN_CPR_AXIS1,
                         encoderCountinCPR);    

    // Set Controller Modes
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_CONTROLLER_MODES_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_CONTROL_MODE_AXIS1,
                        3);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_CONTROLLER_MODES_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_MODE_AXIS1,
                         0);

    // Set Input Pos
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_POS_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_POS_AXIS1,
                         1.524);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_POS_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_VEL_FF_AXIS1,
                         1);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_POS_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_TORQUE_FF_POS_AXIS1,
                         1);

    // Set Input Vel
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_VEL_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_VEL_AXIS1,
                         1.524);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_VEL_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_TORQUE_FF_AXIS1,
                         1.524);

    // Set Input Torque
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_INPUT_TORQUE_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_INPUT_TORQUE_AXIS1,
                         1.524);
    // Set Limits
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_LIMITS_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_VELOCITY_LIMIT_AXIS1,
                         1.524);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_LIMITS_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_CURRENT_LIMIT_AXIS1,
                         1.524);
    // Set Anticogging
    //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_START_ANTICOGGING_AXIS1, 1);

    // Set Traj Vel Limit
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_TRAJ_VEL_LIMIT_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_TRAJ_VEL_LIMIT_AXIS1,
                         1.524);

    // Set Traj Accel Limits
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_TRAJ_ACCEL_LIMIT_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_TRAJ_ACCEL_LIMIT_AXIS1,
                         1.524);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_TRAJ_ACCEL_LIMIT_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_TRAJ_DECEL_LIMIT_AXIS1,
                         1.524);
    // Set Traj Inertia
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_TRAJ_INERTIA_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_TRAJ_INERTIA_AXIS1,
                         1.524);
    
    // Get IQ 
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_IQ_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_IQ_SETPOINT_AXIS1,
                         IqSetpoint);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_IQ_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_IQ_MEASURED_AXIS1,
                         IqMeasured);
    // Get Sensorless Estimates
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_SENSORLESS_ESTIMATES_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_SENSORLESS_POS_ESTIMATE_AXIS1,
                         sensorlessPosEstimate);
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_SENSORLESS_ESTIMATES_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_SENSORLESS_VEL_ESTIMATE_AXIS1,
                         sensorlessVelEstimate);
    // Get Vbus Voltage
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_GET_VBUS_VOLTAGE_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_VBUS_VOLTAGE_AXIS1,
                         vbusVoltage);

    // clear errors
    //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_CLEAR_ERRORS_AXIS1, 1);

    // Set Linear Count
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_LINEAR_COUNT_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_POSITION_AXIS1,
                         0);
    // Set Position Gain
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_POSITION_GAIN_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_POS_GAIN_AXIS1,
                         2.451);
    // Set Vel Gains
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_VEL_GAIN_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_VEL_GAIN_AXIS1,
                         2.451);
    can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_SET_VEL_GAIN_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_VEL_INTEGRATOR_GAIN_AXIS1,
                         2.451);
    
    // Reboot Odrive
    //can.setTXSignalValue(HWBRIDGE::CANID::ODRIVE_REBOOT_ODRIVE_AXIS1, 1);

    // wait until Odrive Responds again
    can.getRXSignalValue(HWBRIDGE::CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS1, HWBRIDGE::CANSIGNAL::ODRIVE_AXIS_CURRENT_STATE_AXIS1,
                         heartbeatCurrentState);*/




}