#include <stm32f4xx_hal.h>

#include "mbed.h"

class HAL_QEI {
 private:
  typedef enum encoderType { relative, absolute } t_encoderType;

  typedef enum quadratureEncodingType { x2_encoding, x4_encoding } t_quadratureEncodingType;

  TIM_Encoder_InitTypeDef m_encoder;
  GPIO_InitTypeDef m_GPIO_InitStruc;
  TIM_HandleTypeDef m_htim;
  TIM_MasterConfigTypeDef m_MasterConfig;

  float m_velocity;
  uint32_t m_pulse1;
  uint32_t m_pulse2;
  uint8_t m_direction;  // if 0 upcounter > forward. if 1 downcounter > backward
  int m_num_averages;
  int m_period;

 public:
  // TODO HAL_QEI(Encoding quadratureEncodingType); implement once testing is complete
  HAL_QEI();
  ~HAL_QEI();
  float readEncoder();
  float getVelocity();
  uint8_t getDirection();
  TIM_HandleTypeDef getTimerObject();

  // TODO: more functionality with encoder
  void setEncoderType();
  void setQuadratureEncodingType();
  void setEncoderPins();
  void setTimerNumber();
};