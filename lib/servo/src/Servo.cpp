//#include "Servo.h" //TODO(qinyang-bao): fix all compile errors and renable servo code
//
//#include <cstdlib>
//#include "mbed.h"
//
// Servo::Servo(PinName pin_, SERVO_TYPE rotate_type_, double value, double max_pulse_, double min_pulse_)
//    : pin(pin_), rotate_type(rotate_type_), max_pulse(max_pulse_), min_pulse(min_pulse_), pwm(pin_) {
//  if (rotate_type == LIM_SERVO) {
//    range     = value;
//    pos       = 0;
//    max_speed = -1;
//    speed     = -1;
//  } else {
//    max_speed = value;
//    speed     = 0;
//    range     = -1;
//    pos       = -1;
//  }
//  // INIT PWM DEFAULT
//  pwm.period_us(PERIOD * 1000);
//}
//
// Servo::Servo(PinName pin_, SERVO_TYPE rotate_type_, double value) : pin(pin_), rotate_type(rotate_type_), pwm(pin_) {
//  if (rotate_type == SERVO_TYPE::LIM_SERVO) {
//    range = value;
//    pos   = 0;
//
//    max_speed = -1;
//    speed     = -1;
//  } else {
//    max_speed = value;
//    speed     = 0;
//
//    range = -1;
//    pos   = -1;
//  }
//
//  max_pulse = DEFAULT_MAX;
//  min_pulse = DEFAULT_MIN;
//
//  // INIT PWM DEFAULT
//  pwm.period_us(PERIOD * 1000);
//}
//
// Servo::Servo(PinName pin_, SERVO_TYPE rotate_type_) : pin(pin_), rotate_type(rotate_type_), pwm(pin_) {
//  if (rotate_type == LIM_SERVO) {
//    range = DEFAULT_RANGE;
//    pos   = 0;
//
//    max_speed = -1;
//    speed     = -1;
//  } else {
//    max_speed = 0;
//    speed     = 0;
//
//    range = -1;
//    pos   = -1;
//  }
//
//  max_pulse = DEFAULT_MAX;
//  min_pulse = DEFAULT_MIN;
//
//  // INIT PWM DEFAULT
//  pwm.period_us(PERIOD * 1000);
//}
//
// bool Servo::set_range(double range_) {
//  if (rotate_type == LIM_SERVO) range = range_;
//  return rotate_type == LIM_SERVO;
//}
//
// bool Servo::set_max_speed(double max_speed_) {
//  if (rotate_type == CONT_SERVO) max_speed = max_speed_;
//  return rotate_type == CONT_SERVO;
//}
//
// bool Servo::set_position(double angle) {
//  if (rotate_type == LIM_SERVO) {
//    pos = angle;
//    pwm.pulsewidth_us(int(((max_pulse - min_pulse) * angle / 180 + min_pulse) * 1000));
//  } else
//    return false;
//}
//
// bool Servo::set_speed(double speed_) {
//  if (rotate_type == CONT_SERVO) {
//    if (std::abs(speed_) > max_speed) speed_ = (speed_ / speed_) * max_speed;
//
//    speed = speed_;
//    pwm.pulsewidth_us(
//        int(((max_pulse - min_pulse) / 2 * speed / max_speed + min_pulse + (max_pulse - min_pulse) / 2) *
//            1000));
//  } else
//    return false;
//}
//
// double Servo::read(void) {
//  if (rotate_type == LIM_SERVO)
//    return pos;
//  else
//    return speed;
//}
//
// void Servo::set_period(int period) {
//  pwm.period_us(period * 1000);
//}
