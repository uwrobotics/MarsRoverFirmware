#pragma once
#include "hw_bridge.h"

// Flags
#define START_FLASH 1UL  // 01
#define ENDED_FLASH 2UL  // 10

class LEDMatrix {
 private:
  DigitalOut m_RChannel;
  DigitalOut m_GChannel;
  DigitalOut m_BChannel;

  HWBRIDGE::LEDMATRIX::color m_flashing_color;
  HWBRIDGE::LEDMATRIX::LEDMatrixState m_led_state;

  volatile bool m_flashing_red, m_flashing_green, m_flashing_blue;
  volatile bool m_continue_flashing;

  EventFlags m_event_flags;
  Thread m_lightsThread;
  static constexpr auto PERIOD_DELAY = 500ms;

  // Thread to take care of flashing.
  void flashing();

  // Turn off all the LEDs. Call setColor(0,0,0).
  void setColor(bool R, bool G, bool B);

  // Start flashing() thread.
  void setFlashColor(bool R, bool G, bool B);

  // Stop flashing() thread and call setColor().
  void setSolidColor(bool R, bool G, bool B);

 public:
  // Define matrix by the pins it is connected to.
  LEDMatrix(PinName R, PinName G, PinName B);

  // Clear lights.
  ~LEDMatrix();

  // Set the state of the LEDs.
  void setState(HWBRIDGE::LEDMATRIX::LEDMatrixState state);
};
