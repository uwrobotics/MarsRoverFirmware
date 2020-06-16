#include "HAL_PWM.h"

// https://www.st.com/resource/en/application_note/dm00236305-generalpurpose-timer-cookbook-for-stm32-microcontrollers-stmicroelectronics.pdf
// https://www.st.com/content/ccc/resource/training/technical/product_training/c4/1b/56/83/3a/a1/47/64/STM32L4_WDG_TIMERS_GPTIM.pdf/files/STM32L4_WDG_TIMERS_GPTIM.pdf/jcr:content/translations/en.STM32L4_WDG_TIMERS_GPTIM.pdf
// https://www.st.com/content/ccc/resource/technical/document/user_manual/2f/71/ba/b8/75/54/47/cf/DM00105879.pdf/files/DM00105879.pdf/jcr:content/translations/en.DM00105879.pdf
// page 868
// https://www.st.com/resource/en/user_manual/dm00154093-description-of-stm32f1-hal-and-lowlayer-drivers-stmicroelectronics.pdf
// http://diyhpl.us/~nmz787/pdf/RM0368__timer_section.pdf

HAL_PWM::HAL_PWM() {
  __TIM1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  m_GPIO_InitStruc.Pin       = GPIO_PIN_8;
  m_GPIO_InitStruc.Mode      = GPIO_MODE_AF_PP;
  m_GPIO_InitStruc.Pull      = GPIO_NOPULL;
  m_GPIO_InitStruc.Speed     = GPIO_SPEED_FAST;
  m_GPIO_InitStruc.Alternate = GPIO_AF1_TIM1;
  HAL_GPIO_Init(GPIOA, &m_GPIO_InitStruc);

  // uses PA_8
  m_htim.Instance           = TIM1;
  m_htim.Init.Prescaler     = 0;
  m_htim.Init.CounterMode   = TIM_COUNTERMODE_UP;
  m_htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  m_htim.Init.Period        = 0xffff;
  HAL_TIM_Base_Init(&m_htim);
  HAL_TIM_IC_Init(&m_htim);

  m_slaveConfig.SlaveMode        = TIM_SLAVEMODE_RESET;
  m_slaveConfig.InputTrigger     = TIM_TS_TI1FP1;
  m_slaveConfig.TriggerPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
  m_slaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
  m_slaveConfig.TriggerFilter    = 0;
  HAL_TIM_SlaveConfigSynchronization(&m_htim, &m_slaveConfig);

  m_timerICConfig.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
  m_timerICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
  m_timerICConfig.ICPrescaler = TIM_ICPSC_DIV1;
  m_timerICConfig.ICFilter    = 0x0;
  HAL_TIM_IC_ConfigChannel(&m_htim, &m_timerICConfig, TIM_CHANNEL_1);

  m_timerICConfig.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
  m_timerICConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  HAL_TIM_IC_ConfigChannel(&m_htim, &m_timerICConfig, TIM_CHANNEL_2);

  m_MasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  m_MasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&m_htim, &m_MasterConfig);

  HAL_TIM_PWM_Start(&m_htim, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&m_htim, TIM_CHANNEL_2);
}

// http://diyhpl.us/~nmz787/pdf/RM0368__timer_section.pdf page 261
void HAL_PWM::calcDutyCycle() {
  while (!__HAL_TIM_GET_FLAG(&m_htim, TIM_FLAG_CC1)) {
    wait_ms(2);
  }

  m_period = HAL_TIM_ReadCapturedValue(&m_htim, TIM_CHANNEL_1);
  __HAL_TIM_CLEAR_FLAG(&m_htim, TIM_FLAG_CC1);

  while (!__HAL_TIM_GET_FLAG(&m_htim, TIM_FLAG_CC2)) {
    wait_ms(2);
  }

  m_pulse_width = HAL_TIM_ReadCapturedValue(&m_htim, TIM_CHANNEL_2);
  __HAL_TIM_CLEAR_FLAG(&m_htim, TIM_FLAG_CC2);

  m_dutyCycle = m_pulse_width / m_period;
}

float HAL_PWM::getDutyCycle() {
  return m_dutyCycle;
}

float HAL_PWM::getPeriod() {
  return m_period;
}

float HAL_PWM::getPulseWidth() {
  return m_pulse_width;
}

TIM_HandleTypeDef HAL_PWM::getTimerObject() {
  return m_htim;
}