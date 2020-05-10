#pragma once

namespace ROS_CONSTANTS{
    namespace ARM{
        namespace ACTUATOR{
            enum{
                TURNTABLE,
                SHOULDER,
                ELBOW,
                WRISTLEFT,
                WRISTRIGHT,
                CLAW
            };
        }
        namespace PID{
            enum{
                P,
                I,
                D
            };
        }
    }
}