#include <stm32f4xx_hal.h>

#include "mbed.h"

class HAL_QEI {
 private:
  typedef enum encoderType { relative, absolute } t_encoderType;

  typedef enum quadratureEncodingType { x2_encoding, x4_encoding } t_quadratureEncodingType;

  TIM_Encoder_InitTypeDef m_encoder;
  GPIO_InitTypeDef m_GPIO_InitStruc;
  TIM_HandleTypeDef m_Tim_BaseInitStruc;
  TIM_MasterConfigTypeDef m_MasterConfig;

  float m_velocity;
  int m_pulse1;
  int m_pulse2;
  char m_direction;

 public:
  // TODO HAL_QEI(Encoding quadratureEncodingType); implement once testing is complete
  HAL_QEI();
  ~HAL_QEI();
  float readEncoder();
  float getVelocity();
  char getDirection();
};