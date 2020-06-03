#include "HAL_QEI.h"

/*
links:
https://stackoverflow.com/questions/15203069/stm32-rotary-encoder-config-on-tim4
https://d1.amobbs.com/bbs_upload782111/files_10/ourdev_265522.pdf
https://www.fmf.uni-lj.si/~ponikvar/STM32F407%20project/Ch9%20-%20Counting%20pulses%20by%20Timer%202.pdf
https://community.st.com/s/question/0D50X00009XkWCY/using-timer-in-encoder-mode-with-interrupts-on-value-change?
*/

HAL_QEI::HAL_QEI()
{
    __TIM2_CLK_ENABLE();
    _GPIOA_CLK_ENABLE();

    m_GPIO_InitStruc.Pin = GPIO_Pin_9;
    m_GPIO_InitStruc.Mode = GPIO_Mode_AF_PP; //may need to select a different mode 
    m_GPIO_InitStruc.Pull = GPIO_NOPULL; //pull up or down or no?
    m_GPIO_InitStruc.Speed = GPIO_SPEED_FAST; //not sure what speed is necedded 
    HAL_GPIO_Init(GPIOA, &m_GPIO_InitStruc);


    m_Tim_BaseInitStruct.Instance = TIM2; //timer to use
    m_Tim_BaseInitStruc.Prescaler = TIM_CLEARINPUTPRESCALER_DIV1; // number of pulse sbefore pulse is passed into CNT
    m_Tim_BaseInitStruc.CounterMode = TIM_COUNTERMODE_UP; 
    m_Tim_BaseInitStruc.Period = 100000; //number of counts until timer resets
    m_Tim_BaseInitStruc.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&m_Tim_BaseInitStruc);
    HAL_TIM_BASE_Start(&m_Tim_BaseInitStruc);

    m_encoder.EncoderMode = TIM_ENCODERMODE_TI12; //initialize encoder to use two channels
    m_encoder.IC1Polarity =  TIM_ICPOLARITY_RISING;  //set rising edge to be the trigger for counting
    m_encoder.IC1Selection =  TIM_ICSELECTION_DIRECTTI;
    m_encoder.IC1Prescaler =  TIM_ICPSC_DIV1; //take in every pulse
    m_encoder.IC1Filter =  0x0f;

    m_encoder.IC2Polarity =  TIM_ICPOLARITY_RISING;
    m_encoder.IC2Selection =  TIM_ICSELECTION_DIRECTTI;
    m_encoder.IC2Prescaler =  TIM_ICPSC_DIV1;
    m_encoder.IC2Filter =  0x0f;

    TIM2->CNT = 0;//initialize counter
    TIM2->CR2 = 0x01;//enable counter, maybe use CEN?


    HAL_TIM_Encoder_Init(&m_Tim_BaseInitStruc, &m_encoder);
    HAL_TIM_Encoder_Start(&m_Tim_BaseInitStruc, TIM_CHANNEL_1);
}

float HAL_QEI::readEncoder()
{
    m_pulse1 = TIM2->CNT; //apparently this will give encoder position and its change is the direction
    //http://diyhpl.us/~nmz787/pdf/RM0368__timer_section.pdf page 275

    waitms(8);

    m_pulse2 = TIM2->CNT;
    m_direction = forward;

    if(m_pulse2 < m_pulse1)
    {
        m_direction = backward;
    }

    m_velocity = (m_pulse2 - m_pulse1)/ 0.008; //change in pulses over time
    return m_velocity;
}
