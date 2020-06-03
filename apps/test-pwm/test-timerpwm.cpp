#include "mbed.h"
#include <stm32f4xx_hal.h>
#include "HAL_PWM.h"

Pwmout outputpin(PA_7);
DigitalOut led(LED1);
Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

//https://electronics.stackexchange.com/questions/179546/getting-pwm-to-work-on-stm32f4-using-sts-hal-libraries
void initializePWM()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_Pin_8;
    GPIO_InitStructure.Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruc);

    __TIM3_CLK_ENABLE();
    TIM_OC_InitTypeDef sConfigOC;
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 0;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);

    TIM_MasterConfigTypeDef sMasterConfig;
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 100;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3 ,TIM_CHANNEL_1);
}

int main()
{
    led = 0;
    pc.printf("Starting pwm generation");
    float expected_period = 2;
    float expected_dutyCycle = 0;

    //output pulses
    outputpin.period(expected_period);

    //start timer to read pwm pulses

    HAL_PWM pwm_Timer();
    while(1)
    {
        //check period, duty cycle, pulsewidth to see if they match
        pc.printf("Period: %f  DutyCycle: %f Pulse Width: %f \n");
        //turn led on if they match
        if(expected_period == pwm_Timer.m_period && expected_dutyCycle == pwm_Timer.m_dutyCycle)
        {
            led = 1;
        }
    }
}