#pragma once
#include "hw_bridge.h"

#define START_FLASH 1UL //01
#define ENDED_FLASH 2UL //10

class LEDMatrix {
  private:
    DigitalOut m_RChannel;
    DigitalOut m_GChannel;
    DigitalOut m_BChannel;

    HWBRIDGE::LEDMATRIX::color flashing_color;
    volatile bool flashing_red, flashing_green, flashing_blue; 
    volatile bool continue_flashing;

    EventFlags event_flags;
    Thread *lightsThread; 
    static constexpr auto PERIOD_DELAY = 500ms;

    void flashing();

    // Turn off all the LEDs. Call setColor(0,0,0).
    void setColor(bool R, bool G, bool B);

    // Call setColor with the enum color.
    void setColor(HWBRIDGE::LEDMATRIX::color c);

  public:
    // Define matrix by the pins it is connected to.
    LEDMatrix(PinName R, PinName G, PinName B);

    // Delete the pointer to the thread if it has not been deleted already. Clear lights.
    ~LEDMatrix();

    void setFlashColor(bool R, bool G, bool B);

    void setSolidColor(bool R, bool G, bool B);

    void clearLights();    
};
