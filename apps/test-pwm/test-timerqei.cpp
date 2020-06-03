#include "mbed.h"
#include <stm32f4xx_hal.h>
#include "HAL_QEI.h"

Pwmout pinA(PA_7);
Pwmout pinB(PA_6);
DigitalOut led(LED1);
Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);


void initializepulses()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_Pin_8 | GPIO_PIN_7;
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
    float expected_period = 2;
    float expected_velocity = 0;

    pc.printf("Starting pwm generation");
    pinA.period(expected_period);
    waitms(50);
    pinb.period(expected_period);
    //initializePWM();

    //initialize encoder
    HAL_QEI quadrature_encoder();
    //read in pulses
    while(1)
    {   
        pc.printf("Velocity: %f Direction: %s");
        //check if velocity is as expected, turn led on if expected
        if(expected_velocity == quadrature_encoder.m_velocity)
        {
            led = 1;
        }
    }
}