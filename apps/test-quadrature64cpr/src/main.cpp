#include "Logger.h"
#include "Quadrature64CPR.h"

void read_all();

// first pin is yellow wire
// second pin is white wire
constexpr Encoder::Quadrature64CPR::Config config = {PA_5, PA_6, NC, 0};
Encoder::Quadrature64CPR enc(config);

Timer theta_timer, theta_dot_timer;
float theta     = 0;
float theta_dot = 0;

int main() {
  while (true) {
    // reset the timers
    theta_timer.reset();
    theta_dot_timer.reset();
    // update the encoder
    if (enc.update()){
      // read and print the angle
      theta = enc.getAngleDeg();
      theta_timer.stop();
      printf("Angle reading took: %lluus\tAngle: %.3f\r\n", theta_timer.elapsed_time().count(), theta);
      // read and print the speed
      theta_dot = enc.getAngularVelocityDegPerSec();
      theta_dot_timer.stop();
      printf("Angular speed reading took: %lluus\tSpeed: %.3f\r\n", theta_dot_timer.elapsed_time().count(), theta_dot);
    }else {
      printf("Error updating the encoder\n");
    }
    ThisThread::sleep_for(500ms);
  }
}
