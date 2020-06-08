#include <stm32f4xx_hal.h>

#include "mbed.h"

class HAL_PWM {
 private:
  // TODO: allow selecting a different timer
  float m_dutyCycle;
  float m_period;
  float m_pulse_width;

  TIM_HandleTypeDef m_htim;
  TIM_IC_InitTypeDef m_timerICConfig;
  TIM_SlaveConfigTypeDef m_slaveConfig;
  TIM_MasterConfigTypeDef m_MasterConfig;
  GPIO_InitTypeDef m_GPIO_InitStruc;

 public:
  HAL_PWM();
  ~HAL_PWM();
  void calcDutyCycle();
  float getDutyCycle();
  float getPeriod();
  float getPulseWidth();
  TIM_HandleTypeDef getTimerObject();
};