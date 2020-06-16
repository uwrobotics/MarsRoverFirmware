#include "HAL_QEI.h"

/*
links:
https://stackoverflow.com/questions/15203069/stm32-rotary-encoder-config-on-tim4
http://www.micromouseonline.com/2013/02/16/quadrature-encoders-with-the-stm32f4/
https://www.fmf.uni-lj.si/~ponikvar/STM32F407%20project/Ch9%20-%20Counting%20pulses%20by%20Timer%202.pdf
https://community.st.com/s/question/0D50X00009XkWCY/using-timer-in-encoder-mode-with-interrupts-on-value-change?
https://www.st.com/content/ccc/resource/technical/document/application_note/54/0f/67/eb/47/34/45/40/DM00042534.pdf/files/DM00042534.pdf/jcr:content/translations/en.DM00042534.pdf
*/

HAL_QEI::HAL_QEI() {
  m_period       = 0x00ff;
  m_num_averages = 1;
  m_pulse1       = 0;
  m_pulse2       = 0;

  __TIM2_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  m_GPIO_InitStruc.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
  m_GPIO_InitStruc.Mode      = GPIO_MODE_AF_PP;
  m_GPIO_InitStruc.Pull      = GPIO_NOPULL;
  m_GPIO_InitStruc.Speed     = GPIO_SPEED_HIGH;
  m_GPIO_InitStruc.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOB, &m_GPIO_InitStruc);

  m_htim.Instance           = TIM2;
  m_htim.Init.Prescaler     = 0;  // number of pulse before pulse is passed into CNT
  m_htim.Init.CounterMode   = TIM_COUNTERMODE_UP;
  m_htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  m_htim.Init.Period        = m_period;  // number of counts until timer resets, can be anything

  HAL_TIM_Base_Init(&m_htim);
  HAL_TIM_Base_Start(&m_htim);

  m_encoder.EncoderMode  = TIM_ENCODERMODE_TI12;   // x2, x4
  m_encoder.IC1Polarity  = TIM_ICPOLARITY_RISING;  // set rising edge to be the trigger for counting
  m_encoder.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  m_encoder.IC1Prescaler = TIM_ICPSC_DIV1;
  m_encoder.IC1Filter    = 0x00;

  m_encoder.IC2Polarity  = TIM_ICPOLARITY_RISING;  // TIM_INPUTCHANNEL_RISING
  m_encoder.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  m_encoder.IC2Prescaler = TIM_ICPSC_DIV1;
  m_encoder.IC2Filter    = 0x00;

  m_htim.Instance->CNT = 0;  // initialize counter
  m_htim.Instance->CR1 = 1;  // enable counter, maybe use CEN?

  HAL_TIM_Encoder_Init(&m_htim, &m_encoder);

  m_MasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  m_MasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&m_htim, &m_MasterConfig);

  HAL_TIM_Encoder_Start(&m_htim, TIM_CHANNEL_1);
  HAL_TIM_Encoder_Start(&m_htim, TIM_CHANNEL_2);
}

float HAL_QEI::readEncoder() {
  // deal with counting up to the end of the timer
  for (int i = 0; i < m_num_averages; i++) {
    // calculate average
    m_pulse1 = m_htim.Instance->CNT;  // apparently this will give encoder position and its change is the direction
    // http://diyhpl.us/~nmz787/pdf/RM0368__timer_section.pdf page 275
    wait_ms(100);
    m_pulse2 = m_htim.Instance->CNT;

    // get direction, determines how to calculate velocity
    getDirection();

    if (m_direction == 0) {
      // forward motion, counter counts up
      if (m_pulse2 < m_pulse1)
        m_velocity += (m_pulse1 - m_pulse2);
      else
        // counter loops so must handle negative values to get accurate counter changes
        m_velocity += ((m_period - m_pulse2) + m_pulse1);
    } else if (m_direction == 16) {
      // backward motion, counter counts down
      if (m_pulse1 < m_pulse2)
        m_velocity += (m_pulse2 - m_pulse1);
      else
        m_velocity += ((m_period - m_pulse1) + m_pulse2);
    }
  }
  m_velocity /= m_num_averages;  // may need to multiple by a calibration factor to convert the counter to m/s

  return m_velocity;
}

float HAL_QEI::getVelocity() {
  return m_velocity;
}

// timer CR1 register contains the direction of the counter as the 5th bit. 0 is forward, 1 is backward
uint8_t HAL_QEI::getDirection() {
  m_direction = m_htim.Instance->CR1 & (1 << 4);
  return m_direction;
}

TIM_HandleTypeDef HAL_QEI::getTimerObject() {
  return m_htim;
}
