#include "mbed.h"
#include <stm32f4xx_hal.h>

enum DIRECTION{
    forward,
    backward
}

class HAL_QEI{
    public:
        typedef enum encoderType {
            relative,
            absolute
        } t_encoderType;

        typedef enum quadratureEncodingType {
            x2_encoding,
            x4_encoding
        } t_quadratureEncodingType;

        TIM_Encoder_InitTypeDef m_encoder;
        GPIO_InitTypedef m_GPIO_InitStruc;
        TIM_HandleTypeDef m_Tim_BaseInitStruc;

        float m_velocity;
        int m_pulse1;
        int m_pulse2;
        int m_direction;

    private:
        HAL_QEI(Encoding quadratureEncodingType);
        ~HAL_QEI();
        float readEncoder();
}