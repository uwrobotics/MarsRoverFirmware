#include "PID.h"

constexpr uint8_t arbitrary_gain = 10;
constexpr uint8_t min_rpm = 0, max_rpm = 200;
constexpr uint8_t deadzone  = 1;  // 1% precision
constexpr auto pid_period = 500ms;   // 1kHz

// limit dependencies of this test driver
typedef struct motor_t {
  DigitalOut dir;
  PwmOut speed;
  motor_t(PinName _dir, PinName _speed) : dir(_dir), speed(_speed) {
    dir.write(1);
  }
} motor_t;

typedef struct encoder_t {
  encoder_t(PinName ch1, PinName ch2) : encoderCh1(ch1), encoderCh2(ch2) {}
  int read();
  InterruptIn encoderCh1;
  InterruptIn encoderCh2;
} encoder_t;

// every time a pulse is received from the encoder channels, increase the pulse count
void countPulses();

int main() {
  printf("##################### PID TEST APP STARTED #####################\r\n");
  PID::Pid controller(arbitrary_gain, arbitrary_gain, arbitrary_gain, min_rpm, max_rpm, deadzone);
  motor_t motor(D8, D9);
  encoder_t encoder(D2, D3);
  
  Timer timer;

  while(true) {
    for(uint8_t i = 0; i < max_rpm; i+=10) {
      timer.start();
      float speed = controller.compute(i, encoder.read());
      motor.speed(speed);
      timer.stop();
      auto elapsed = timer.read();
      ThisThread::sleep_for(pid_period-elapsed > 0 ? pid_period-elapsed : 0);
    }
    timer.reset();
  }
}
