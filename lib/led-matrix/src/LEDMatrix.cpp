#include "LEDMatrix.h"

#include "mbed.h"

LEDMatrix::LEDMatrix(PinName R, PinName G, PinName B)
    : m_RChannel(R),
      m_GChannel(G),
      m_BChannel(B),
      m_flashing_red(false),
      m_flashing_green(false),
      m_flashing_blue(false),
      m_continue_flashing(false) {
  m_lightsThread.start(callback(this, &LEDMatrix::flashing));
}

LEDMatrix::~LEDMatrix() {
  m_lightsThread.terminate();
  setSolidColor(0, 0, 0);
}

void LEDMatrix::setState(HWBRIDGE::LEDMATRIX::LEDMatrixState state) {
  switch (state) {
    case HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_RED:
      setSolidColor(1, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_BLUE:
      setSolidColor(0, 0, 1);
      break;
    case HWBRIDGE::LEDMATRIX::LEDMatrixState::SOLID_GREEN:
      setSolidColor(0, 1, 0);
      break;
    case HWBRIDGE::LEDMATRIX::LEDMatrixState::FLASHING_RED:
      setFlashColor(1, 0, 0);
      break;
    case HWBRIDGE::LEDMATRIX::LEDMatrixState::FLASHING_BLUE:
      setFlashColor(0, 0, 1);
      break;
    case HWBRIDGE::LEDMATRIX::LEDMatrixState::FLASHING_GREEN:
      setFlashColor(0, 1, 0);
      break;
    case HWBRIDGE::LEDMATRIX::LEDMatrixState::OFF:
      setSolidColor(0, 0, 0);
      break;
  }
}

void LEDMatrix::flashing() {  // check https://os.mbed.com/docs/mbed-os/v6.8/apis/thisthread.html
  while (true) {
    m_event_flags.wait_all(START_FLASH);  // yield until flag is set
    m_continue_flashing = true;

    while (m_continue_flashing) {
      setColor(m_flashing_red, m_flashing_green, m_flashing_blue);
      ThisThread::sleep_for(PERIOD_DELAY);
      setColor(0, 0, 0);
      ThisThread::sleep_for(PERIOD_DELAY);
    }
    m_event_flags.set(ENDED_FLASH);
  }
}

void LEDMatrix::setFlashColor(bool R, bool G, bool B) {
  m_flashing_red   = R;
  m_flashing_green = G;
  m_flashing_blue  = B;

  if (!m_continue_flashing) {
    m_event_flags.set(START_FLASH);
  }
}

void LEDMatrix::setSolidColor(bool R, bool G, bool B) {
  if (m_continue_flashing) {
    m_continue_flashing = false;
    m_event_flags.wait_all(ENDED_FLASH);  // wait for while loop in flashing() to finish
  }
  setColor(R, G, B);
}

void LEDMatrix::setColor(bool R, bool G, bool B) {
  m_RChannel = R;
  m_GChannel = G;
  m_BChannel = B;
}
