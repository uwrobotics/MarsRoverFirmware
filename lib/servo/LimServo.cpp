#include "LimServo.h"

LimServo::LimServo(PinName pin): Servo(pin)
{
    m_rotate_type = LIM_SERVO;

	m_range = DEFAULT_RANGE;
	m_pos = 0;

	m_max_pulse_ms = DEFAULT_MAX;
	m_min_pulse_ms = DEFAULT_MIN;

    //INIT PWM DEFAULT
    m_pwm.period_us(PERIOD*1000);
}

LimServo::LimServo(PinName pin, float range): LimServo(pin)
{
    m_range = range;
}

LimServo::LimServo(PinName pin, float range, float max_pulse_ms, float min_pulse_ms): LimServo(pin, range)
{
    m_max_pulse_ms = max_pulse_ms;
    m_min_pulse_ms = min_pulse_ms;
}

bool LimServo::setRange(float range){
    m_range = range;
    return true;
}

float LimServo::getRange(void){
    return m_range;
}

bool LimServo::setPosition(float angle){
    m_pos = (std::abs(angle) < m_range)? angle : m_range;
    m_pwm.pulsewidth_us(int(((m_max_pulse_ms - m_min_pulse_ms) * angle/180.0 + m_min_pulse_ms)*1000));
    return true;
}

float LimServo::read(void){
    return m_pos;
}
