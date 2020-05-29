#include "mbed.h"
#include <stm32f4xx_hal.h>


/*
    capture input -> set flag -> poll flag -> count flags?

    may use Timer encoder mode, has 2 input channels possibly best for motor encoder

    htim.instance->CNT

    hall sensor or not hall sensor?

links:
https://stackoverflow.com/questions/15203069/stm32-rotary-encoder-config-on-tim4
https://d1.amobbs.com/bbs_upload782111/files_10/ourdev_265522.pdf
https://www.fmf.uni-lj.si/~ponikvar/STM32F407%20project/Ch9%20-%20Counting%20pulses%20by%20Timer%202.pdf
https://community.st.com/s/question/0D50X00009XkWCY/using-timer-in-encoder-mode-with-interrupts-on-value-change?
*/

void initialize_timer()
{
    GPIO_InitTypedef GPIO_InitStruc;
    __TIM2_CLK_ENABLE();
    _GPIOA_CLK_ENABLE();

    GPIO_InitStruc.Pin = GPIO_Pin_9;
    GPIO_InitStruc.Mode = GPIO_Mode_AF_PP; //may need to select a different mode 
    GPIO_InitStruc.Pull = GPIO_NOPULL; //pull up or down or no?
    GPIO_InitStruc.Speed = GPIO_SPEED_FAST; //not sure what speed is necedded 
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruc);

    TIM_HandleTypeDef Tim_BaseInitStruc;

    Tim_BaseInitStruct.Instance = TIM2; //timer to use
    Tim_BaseInitStruc.Prescaler = TIM_CLEARINPUTPRESCALER_DIV1; // number of pulse sbefore pulse is passed into CNT
    Tim_BaseInitStruc.CounterMode = TIM_COUNTERMODE_UP; 
    Tim_BaseInitStruc.Period = 100000; //number of counts until timer resets
    Tim_BaseInitStruc.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&Tim_BaseInitStruc);
    HAL_TIM_BASE_Start(&Tim_BaseInitStruc);


    TIM_Encoder_InitTypeDef encoder;
    encoder.EncoderMode = TIM_ENCODERMODE_TI12; //initialize encoder to use two channels
    encoder.IC1Polarity =  TIM_ICPOLARITY_RISING;  //set rising edge to be the trigger for counting
    encoder.IC1Selection =  TIM_ICSELECTION_DIRECTTI;
    encoder.IC1Prescaler =  TIM_ICPSC_DIV1; //take in every pulse
    encoder.IC1Filter =  0x0f;
    encoder.IC2Polarity =  TIM_ICPOLARITY_RISING;
    encoder.IC2Selection =  TIM_ICSELECTION_DIRECTTI;
    encoder.IC2Prescaler =  TIM_ICPSC_DIV1;
    encoder.IC2Filter =  0x0f;

    TIM2->CNT = 0;//initialize counter
    TIM2->CR1 = 1;//enable counter


    HAL_TIM_Encoder_Init(&Tim_BaseInitStruc, &encoder);
    HAL_TIM_Encoder_Start(&Tim_BaseInitStruc, TIM_CHANNEL_1);
}

int readEncoder()
[
    return TIM2->CNT; //apparently this will give encoder position
]
//reading the timer's counter