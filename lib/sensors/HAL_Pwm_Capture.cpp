#include "HAL_PWM.h"

// https://www.st.com/resource/en/application_note/dm00236305-generalpurpose-timer-cookbook-for-stm32-microcontrollers-stmicroelectronics.pdf
// https://www.st.com/content/ccc/resource/training/technical/product_training/c4/1b/56/83/3a/a1/47/64/STM32L4_WDG_TIMERS_GPTIM.pdf/files/STM32L4_WDG_TIMERS_GPTIM.pdf/jcr:content/translations/en.STM32L4_WDG_TIMERS_GPTIM.pdf
//https://www.st.com/content/ccc/resource/technical/document/user_manual/2f/71/ba/b8/75/54/47/cf/DM00105879.pdf/files/DM00105879.pdf/jcr:content/translations/en.DM00105879.pdf page 868
//https://www.st.com/resource/en/user_manual/dm00154093-description-of-stm32f1-hal-and-lowlayer-drivers-stmicroelectronics.pdf
//http://diyhpl.us/~nmz787/pdf/RM0368__timer_section.pdf 

/*
    timer gets an event from input pin
    timer stores the timestamp in a channel
    timer sets the flag register
    read from that channel register if the flag register is set
*/


HAL_PWM::HAL_PWM()
{
    __TIM1_CLK_ENABLE();

    m_htim.Instance = TIM1;
    m_htim.Init.Prescaler = 0;
    m_htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    m_htim.Init.Period = 65535;
    HAL_TIM_IC_Init(&m_htim);

    m_slaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
    m_slaveConfig.InputTrigger = TIM_TS_TI2FP2;
    m_slaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    m_slaveConfig.TriggerPrescaler = TIM_ICPSC_DIV1;
    m_slaveConfig.TriggerFilter = 0;
    HAL_TIM_SlaveConfigSynchronization(&m_htim, &m_sSlaveConfig);

    m_timerICConfig.Init.ICPolarity = TIM_ICPOLARITY_RISING; //going to need width of pulse to calc duty cycle
    m_timerICConfig.Init.ICSelection = TIM_ICSELECTION_DIRECTTI;
    m_timerICConfig.Init.ICPrescaler = TIM_ICPSC_DIV1;
    m_timerICConfig.Init.ICFilter = 0x0;
    HAL_TIM_IC_ConfigChannel(&m_htim, &m_timerICConfig, TIM_CHANNEL_1);

    timerICConfig.Init.ICPolarity = TIM_ICPOLARITY_FALLING; //going to need width of pulse to calc duty cycle
    timerICConfig.Init.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    HAL_TIM_IC_ConfigChannel(&m_htim, &m_timerICConfig, TIM_CHANNEL_2);

    HAL_TIM_IC_Start(&m_htim, TIM_CHANNEL_1);
    HAL_TIM_IC_Start(&m_htim, TIM_CHANNEL_2);
}
  
//http://diyhpl.us/~nmz787/pdf/RM0368__timer_section.pdf page 261
void HAL_PWM::calcDutyCycle()
{
    while(!__HAL_TIM_GET_FLAG(&m_htim, TIM_FLAG_CC1))//also maybe use TIMx->SR
    {
        waitms(2);
    }

    m_period = HAL_TIM_ReadCapturedValue(&m_htim, TIM_CHANNEL_1); //__HAL_TIM_GET_COMPARE(&Timer, TIM_CHANNEL_1)
    __HAL_TIM_CLEAR_FLAG(&m_htim, TIM_FLAG_CC1);

    while(!__HAL_TIM_GET_FLAG(&m_htim, TIM_FLAG_CC2))//also maybe use TIMx->SR
    {
        waitms(2);
    }

    m_pulse_width = HAL_TIM_ReadCapturedValue(&m_htim, TIM_CHANNEL_2);
    __HAL_TIM_CLEAR_FLAG(&m_htim, TIM_FLAG_CC2);

    m_dutyCycle = m_pulse_width / m_period;
}

/*
absolute and relative encoders
science and arm

libraries use interrupts 
can set timer to different channels
look at timer hardware for up and down counting  

qei count edges 
period is 4 ms(2 edges in 4 ms)

average values not on every interrupt
lots of variation for pwm, less for qei
stm32 quadrature mode

maybe use cubemx to see what code is created
*/
