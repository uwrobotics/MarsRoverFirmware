#include "mbed.h"
#include <stm32f4xx_hal.h>

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

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t IC_Val3 = 0;


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM1)
    {
        //do something to record value
        IC_Val1 = HAL_TIM_ReadCapturedValue(&Timer, TIM_CHANNEL_1);

        IC_Val2 = HAL_TIM_ReadCapturedValue(&Timer, TIM_CHANNEL_1);



        float pulseWidth_s = IC_Val2 - IC_Val1;
        float period_s = 0;
        float dutyCycle = pulseWidth_s * 100 / period_s;
    }


}
//ask about how we want this system to run. Int eh background all the time? Whenever we have pulses?


void initializeTimer()
{
    __TIM1_CLK_ENABLE();
    TIM_IC_InitTypeDef timerICConfig;

    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 0;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;


    Timer.Init.ICPolarity = TIM_ICPOLARITY_BOTHEDGE; //going to need width of pulse to calc duty cycle
    Timer.Init.ICSelection = TIM_ICSELECTION_DIRECTTI;
    Timer.Init.ICPrescaler = TIM_ICPSC_DIV1;
    Timer.Init.ICFilter = 0x0;
    HAL_TIM_IC_Init(&Timer);
    HAL_TIM_IC_Start(&Timer, TIM_CHANNEL_1);
}
  
    HAL_TIM_IC_Stop(&Timer, TIM_CHANNEL_1);


    HAL_TIM_IC_DeInit(&Timer);

__HAL_TIM_GET_FLAG(&Timer, TIM_FLAG_CC1) //also maybe use TIMx->SR
__HAL_TIM_GET_COMPARE(&Timer, TIM_CHANNEL_1)

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