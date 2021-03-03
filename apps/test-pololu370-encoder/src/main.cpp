#include "Logger.h"
#include "Pololu37D.h"

void read_all();

// first pin is yellow wire
// second pin is white wire
constexpr Encoder::Pololu37D::Config config = {PA_5, PA_6, NC, 0, GPIO::QEI::Encoding::X2_ENCODING};
Encoder::Pololu37D enc(config);

Timer theta_timer, theta_dot_timer;
float theta     = 0;
float theta_dot = 0;

int main() {
  while (true) {
    theta_timer.reset();
    theta_dot_timer.reset();
    read_all();
  }
}

void read_all() {
  // angle reading
  theta_timer.start();
  if (enc.getAngleDeg(theta)) {
    theta_timer.stop();
    Utility::Logger::printf("Angle reading took: %lluus\tAngle: %.3f\r\n", theta_timer.elapsed_time().count(), theta);
  } else {
    Utility::Logger::printf("an error occured :(\n");
  }
  ThisThread::sleep_for(500ms);
  // speed reading
  theta_dot_timer.start();
  if (enc.getAngularVelocityDegPerSec(theta_dot)) {
    theta_dot_timer.stop();
    Utility::Logger::printf("Angular speed reading took: %lluus\tSpeed: %.3f\r\n",
                            theta_dot_timer.elapsed_time().count(), theta_dot);
  } else {
    Utility::Logger::printf("an error occured :(\n");
  }
  ThisThread::sleep_for(500ms);
}