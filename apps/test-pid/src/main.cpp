#include "PID.h"
#include "mbed.h"

// Defines
#define MIN_RPM              0  // change min/max RPMs based on motor used
#define MAX_RPM              300
#define COUNTS_PER_REV       1200   // motor property
#define TIMER_INTERRUPT_FREQ 250ms  // frequency of timer interrupt for input calculation
#define GOAL_RPM             100.0
#define K_UPDATE_PERIOD      150ms

// motor direction pin and pwm out pin, modify as needed
DigitalOut MOTOR_DIR(D8);
PwmOut MOTOR_PWM_OUT(D9);

// interrupt in to count encoder rises/falls
InterruptIn encoderCh1(D2);
InterruptIn encoderCh2(D3);

PID rpmPIDController(0.010268, 0.0260, 0, K_UPDATE_PERIOD);
Timer timer;

// Variables
uint64_t pulseCount    = 0;
uint64_t oldPulseCount = 0;
float motorRPM         = 0.0;
float motorPWMDuty     = 0.0;
Ticker interruptTimer;

// PID AutoTune config struct for specific DC motor, change depending on actuator
PID::t_AutoTuneConfig autoTuneConfig = {
    .nLookBack = 40, .sampleTime = 250ms, .outputStart = 0.4, .oStep = 0.25, .noiseBand = 0.01, .setpoint = 120};

// Setup velocity PID controller
void initializePidController(void) {
  rpmPIDController.setInputLimits(MIN_RPM, MAX_RPM);
  rpmPIDController.setOutputLimits(0.0, 1.0);
  rpmPIDController.setBias(0.0);
  rpmPIDController.setMode(PID_AUTO_MODE);
  rpmPIDController.setupAutoTune(&MOTOR_PWM_OUT, &motorRPM, 0);
}

// every time a pulse is received from the encoder channels, increase the pulse count
void countPulses() {
  pulseCount++;
}

// every timer interrupt, recompute the rpm
void computeInput() {
  motorRPM = (pulseCount - oldPulseCount) /
             std::chrono::duration_cast<std::chrono::duration<float, std::ratio<60>>>(TIMER_INTERRUPT_FREQ).count() /
             COUNTS_PER_REV;
  oldPulseCount = pulseCount;
}

int main() {
  encoderCh1.rise(&countPulses);  // attach the address of the pulse count function to the rising edge
  encoderCh1.fall(&countPulses);
  encoderCh2.rise(&countPulses);
  encoderCh2.fall(&countPulses);

  printf("PID Test - Start \r\n");

  // Initialization
  std::chrono::duration<float> interval = 0.1s;
  initializePidController();
  rpmPIDController.setSetPoint(GOAL_RPM);                      // Set RPM set point
  MOTOR_DIR = 1;                                               // set default direction
  interruptTimer.attach(&computeInput, TIMER_INTERRUPT_FREQ);  // attach function to timer interrupt
  timer.start();

  // uncomment below line and comment line after for debug output
  // rpmPIDController.autoTune(&pc, true, &autoTuneConfig);
  rpmPIDController.autoTune(true, &autoTuneConfig);

  printf("Autotune Params obtained: Kc: %f \t    TauI: %f \t    TauD: %f \r\n", rpmPIDController.getATunePParam(),
         rpmPIDController.getATuneIParam(), rpmPIDController.getATuneDParam());
  rpmPIDController.setAutoTuneParams();
  interruptTimer.detach();

  ThisThread::sleep_for(5s);

  Timer eval;
  eval.start();

  while (1) {
    motorRPM = (pulseCount - oldPulseCount) *
               std::chrono::duration_cast<std::chrono::duration<float, std::ratio<60>>>(interval).count() /
               COUNTS_PER_REV;
    oldPulseCount = pulseCount;

    // Update the PID controller
    rpmPIDController.setInterval(interval);
    rpmPIDController.setProcessValue(motorRPM);
    motorPWMDuty  = rpmPIDController.compute();
    MOTOR_PWM_OUT = motorPWMDuty;

    printf("Motor RPM: %f, \t Goal RPM: %f, \t PWM Output: %f\r\n", motorRPM, GOAL_RPM, motorPWMDuty);
    if (abs(motorRPM - GOAL_RPM) < 1.0) {
      printf("Time taken to reach goal RPM: %f seconds \r\n",
             std::chrono::duration_cast<std::chrono::duration<float>>(eval.elapsed_time()).count());
      MOTOR_DIR = 0;
      return 0;
    }

    ThisThread::sleep_for(K_UPDATE_PERIOD);
    interval = std::chrono::duration_cast<std::chrono::duration<float>>(timer.elapsed_time());
    timer.reset();
  }
}
