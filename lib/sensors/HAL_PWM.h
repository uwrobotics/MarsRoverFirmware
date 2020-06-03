#include "mbed.h"
#include <stm32f4xx_hal.h>

class HAL_PWM{
    public:
        float m_dutyCycle;
        float m_period;
        float m_pulse_width;
        TIM_HandleTypeDef m_htim;
        TIM_IC_InitTypeDef m_timerICConfig;
        TIM_SlaveConfigTypeDef m_slaveConfig;

    private:
        HAL_PWM();
        ~HAL_PWM();
        void calcDutyCycle();
}