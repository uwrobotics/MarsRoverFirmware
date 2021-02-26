#include "Pololu37D.h"

int main() {
  // first pin is yellow wire
  // second pin is white wire
  constexpr Encoder::Pololu37D::Config config = {PA_5, PA_6, NC, 0, GPIO::QEI::Encoding::X2_ENCODING};
  Encoder::Pololu37D enc(config);

  Timer theta_timer, theta_dot_timer;
  float theta     = 0;
  float theta_dot = 0;
  while (true) {
    theta_timer.reset();
    theta_dot_timer.reset();

    // try reading the encoder
    if (enc.read()) {
      printf("Time since last reading: %.3fms\tAngle: %.3f\r\n", enc.return_delta_time(), theta);
      // angle reading
      theta_timer.start();
      if (enc.getAngleDeg(theta)) {
        theta_timer.stop();
        printf("Angle reading took: %lluus\tAngle: %.3f\r\n", theta_timer.elapsed_time().count(), theta);
      } else {
        printf("an error occured :(");
      }
      // speed reading
      theta_dot_timer.start();
      if (enc.getAngularVelocityDegPerSec(theta_dot)) {
        theta_dot_timer.stop();
        printf("Angular speed reading took: %lluus\tSpeed: %.3f\r\n", theta_dot_timer.elapsed_time().count(),
               theta_dot);
      } else {
        printf("an error occured :(");
      }
    } else {
      printf("an error occured :(");
    }

    ThisThread::sleep_for(500ms);
  }
}