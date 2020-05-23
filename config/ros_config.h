#pragma once

namespace ROS_INTERFACE {
    namespace ARM {
        namespace API {
            struct __attribute__ ((__packed__)) payload_st{
                float value;
                bool velocity;
                uint8_t actuatorID;
            };
        }
        namespace ACTUATOR{
            enum {
                TURNTABLE,
                SHOULDER,
                ELBOW,
                WRISTLEFT,
                WRISTRIGHT,
                CLAW
            };
            std::string stringifyActuatorID(uint8_t actuatorID) {
                switch(actuatorID) {
                    case(ROS_CONSTANTS::ARM::ACTUATOR::TURNTABLE): return "TURNTABLE";
                    case(ROS_CONSTANTS::ARM::ACTUATOR::SHOULDER): return"SHOULDER";
                    case(ROS_CONSTANTS::ARM::ACTUATOR::ELBOW): return "ELBOW";
                    case(ROS_CONSTANTS::ARM::ACTUATOR::WRISTLEFT): return "WRISTLEFT";
                    case(ROS_CONSTANTS::ARM::ACTUATOR::WRISTRIGHT): return "WRISTRIGHT";
                    case(ROS_CONSTANTS::ARM::ACTUATOR::CLAW) : return "CLAW";
                    default: return "ERROR";
                }
            }
        }
        namespace PID {
            enum {
                P,
                I,
                D,
                DEADZONE,
                BIAS
            };
            std::string stringifyParam(uint8_t param) {
                switch(param) {
                    case(ROS_CONSTANTS::ARM::PID::P): return "P";
                    case(ROS_CONSTANTS::ARM::PID::I): return "I";
                    case(ROS_CONSTANTS::ARM::PID::D): return "D";
                    case(ROS_CONSTANTS::ARM::PID::DEADZONE): return "DEADZONE";
                    case(ROS_CONSTANTS::ARM::PID::BIAS): return "BIAS";
                    default: return "ERROR";
                }
            }
            std::string stringifyVelPos(bool vel_pos) {
                return vel_pos ? "velocity" : "position";
            }
        }
    }
}