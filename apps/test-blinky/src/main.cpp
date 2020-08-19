
#include "mbed.h"

CAN can(CAN1_RX, CAN1_TX);


class TutorialServo{

  private:

        PwmOut m_servoPwmOut;
        float m_servoRangeInDegrees;
        float m_minPulsewidthInMs;
        float m_maxPulsewidthInMs;

  public:

        TutorialServo(PinName servoPin, float servoRangeInDegrees = 180, float minP$
                 m_servoRangeInDegrees =  servoRangeInDegrees;
                 m_minPulsewidthInMs = minPulsewidthInMs;
                 m_maxPulsewidthInMs = maxPulsewidthInMs;

        }

        void setAngleRangeInDegrees(float degrees){

                m_servoRangeInDegrees = degrees;
        }

        void setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs){
          
                m_minPulsewidthInMs = minPulsewidthMs;
                m_maxPulsewidthInMs = maxPulsewidthMs;

        }

        void setPositionInDegrees(float degrees){

                float slope{0.0}, pulse{0.0};
                slope = (m_maxPulsewidthInMs - m_minPulsewidthInMs)/(m_servoRangeIn$                pulse = (m_minPulsewidthInMs/1000) + degrees/slope;
                m_servoPwmOut.pulsewidth(pulse);

        }

        float getRangeInDegrees(){

                return m_servoRangeInDegrees;
          
        }

};

int main() {

        TutorialServo servo(PA_1);
        CANMessage msg;

        while (true) {
                if(can.read(msg)){

                        printf("Message received: %d\n", msg.data[0]);
                        float range{servo.getRangeInDegrees()};
                        servo.setPositionInDegrees(range * msg.data[0]);
                }
        }
        return 0;
}
