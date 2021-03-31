#pragma once
#include "hw_bridge.h"

// Flags
#define START_FLASH 1UL  // 01
#define ENDED_FLASH 2UL  // 10

class LEDMatrix {
 public:
  // Define LED matrix colour channels by the pins it is connected to.
  LEDMatrix(PinName R, PinName G, PinName B);

  // Terminate thread and clear lights.
  ~LEDMatrix();

  // Set the state of the LEDs.
  void setState(HWBRIDGE::LEDMATRIX::LEDMatrixState state);

  // Periodically flash the colour given by R, G, B on the LED matrix.
  void setFlashColor(bool R, bool G, bool B);

  // Set a solid colour given by the R, G, B on the LED matrix.
  void setSolidColor(bool R, bool G, bool B);

 private:
  DigitalOut m_RChannel;
  DigitalOut m_GChannel;
  DigitalOut m_BChannel;

  volatile bool m_flashing_red, m_flashing_green, m_flashing_blue;
  volatile bool m_continue_flashing;

  EventFlags m_event_flags;
  Thread m_lightsThread;
  static constexpr auto PERIOD_DELAY = 500ms;

  // Thread to take care of flashing colours on the LED matrix by setFlashColor().
  void flashing();

  // Set the given R, G, B colour channels.
  void setColor(bool R, bool G, bool B);
};
