/**
 * @author Aaron Berk
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * A PID controller is a widely used feedback controller commonly found in
 * industry.
 *
 * This library is a port of Brett Beauregard's Arduino PID library:
 *
 *  http://www.arduino.cc/playground/Code/PIDLibrary
 *
 * The wikipedia article on PID controllers is a good place to start on
 * understanding how they work:
 *
 *  http://en.wikipedia.org/wiki/PID_controller
 *
 * For a clear and elegant explanation of how to implement and tune a
 * controller, the controlguru website by Douglas J. Cooper (who also happened
 * to be Brett's controls professor) is an excellent reference:
 *
 *  http://www.controlguru.com/
 *
 * Additions to original PID library:
 *
 * Autotuning capability, based on this library:
 * https://github.com/br3ttb/Arduino-PID-AutoTune-Library
 *
 * Associated blog post:
 * http://brettbeauregard.com/blog/2012/01/arduino-pid-autotune-library/
 *
 * General usage for autotuning:
 * 1. Create function that calculates required input and attach to a timer
 * interrupt so it can run in the background (or multithread it)
 * 2. Add actuator type to enum in PID class if it doesn't exist
 * 3. Add type cast for actuator type in setOutput() function if it doesn't exist
 * 4. Using PID object call setupAutoTune() passing it I/O pointers and actuator type
 * 5. Call autoTune() function, choose whether you want PI or PID control. Also can
 * pass a autoTuneConfig struct for autotune params (if nothing is passed, default
 * will be used).
 * 6. Highly recommend monitoring autotune loop, try to change outputStart and oStep
 * such that the output oscillates evenly about the setpoint
 * 7. Choose whether or not to use autoTune parameters, can set using
 * setAutoTuneParams() function
 *
 * General note: Don't fully rely on autoTune params, use this as a starting point
 * for finding the right PID tuning params and test if they work well or not
 */

/**
 * Includes
 */

#include "PID.h"

PID::PID(double Kc, double tauI, double tauD, std::chrono::duration<double> interval) {
  usingFeedForward = false;
  inAuto           = false;

  // Default the limits to the full range of I/O: 3.3V
  // Make sure to set these to more appropriate limits for
  // your application.
  setInputLimits(0.0, 3.3);
  setOutputLimits(0.0, 3.3);

  tSample_ = interval;

  setTunings(Kc, tauI, tauD);

  setPoint_             = 0.0;
  processVariable_      = 0.0;
  prevProcessVariable_  = 0.0;
  controllerOutput_     = 0.0;
  prevControllerOutput_ = 0.0;

  accError_      = 0.0;
  deadZoneError_ = 0.0;
  bias_          = 0.0;

  realOutput_ = 0.0;
}

void PID::setInputLimits(double inMin, double inMax) {
  // Make sure we haven't been given impossible values.
  if (inMin >= inMax) {
    return;
  }

  inSpan_ = inMax - inMin;

  // Rescale the working variables to reflect the changes.
  prevProcessVariable_ *= (inMax - inMin) / inSpan_;
  accError_ *= (inMax - inMin) / inSpan_;

  // Make sure the working variables are within the new limits.
  if (prevProcessVariable_ > 1) {
    prevProcessVariable_ = 1;
  } else if (prevProcessVariable_ < 0) {
    prevProcessVariable_ = 0;
  }

  inMin_ = inMin;
  inMax_ = inMax;
}

void PID::setOutputLimits(double outMin, double outMax) {
  // Make sure we haven't been given impossible values.
  if (outMin >= outMax) {
    return;
  }

  // Rescale the working variables to reflect the changes.
  prevControllerOutput_ *= (outMax - outMin) / outSpan_;

  // Make sure the working variables are within the new limits.
  if (prevControllerOutput_ > 1) {
    prevControllerOutput_ = 1;
  } else if (prevControllerOutput_ < 0) {
    prevControllerOutput_ = 0;
  }

  outMin_  = outMin;
  outMax_  = outMax;
  outSpan_ = outMax - outMin;
}

void PID::setTunings(double Kc, double tauI, double tauD) {
  // Verify that the tunings make sense.
  if (Kc == 0.0 || tauI < 0.0 || tauD < 0.0) {
    return;
  }

  // Store raw values to hand back to user on request.
  pParam_ = Kc;
  iParam_ = tauI;
  dParam_ = tauD;

  double tempTauR;

  if (tauI == 0.0) {
    tempTauR = 0.0;
  } else {
    tempTauR = (1.0 / tauI) * tSample_.count();
  }

  // For "bumpless transfer" we need to rescale the accumulated error.
  if (inAuto) {
    if (tempTauR == 0.0) {
      accError_ = 0.0;
    } else {
      accError_ *= (Kc_ * tauR_) / (Kc * tempTauR);
    }
  }

  Kc_   = Kc;
  tauR_ = tempTauR;
  tauD_ = tauD / tSample_.count();
}

void PID::reset(void) {
  double scaledBias = 0.0;

  if (usingFeedForward) {
    scaledBias = (bias_ - outMin_) / outSpan_;
  } else {
    scaledBias = (realOutput_ - outMin_) / outSpan_;
  }

  prevControllerOutput_ = scaledBias;
  prevProcessVariable_  = (processVariable_ - inMin_) / inSpan_;

  // Clear any error in the integral.
  accError_ = 0;
}

void PID::setMode(int mode) {
  // We were in manual, and we just got set to auto.
  // Reset the controller internals.
  if (mode != 0 && !inAuto) {
    reset();
  }

  inAuto = (mode != 0);
}

void PID::setInterval(std::chrono::duration<double> interval) {
  if (interval > 0s) {
    // Convert the time-based tunings to reflect this change.
    tauR_ *= (interval / tSample_);
    accError_ *= (tSample_ / interval);
    tauD_ *= (interval / tSample_);
    tSample_ = interval;
  }
}

void PID::setSetPoint(double sp) {
  setPoint_ = sp;
}

void PID::setProcessValue(double pv) {
  processVariable_ = pv;
}

void PID::setBias(double bias) {
  bias_            = bias;
  usingFeedForward = 1;
}

void PID::setDeadZoneError(double error) {
  deadZoneError_ = error;
}

void PID::setRealOutput(double realOutput) {
  realOutput_ = realOutput;
}

void PID::setupAutoTune(void *outputPointer, double *inputPointer, int actuatorType) {
  // set actuator type
  actuatorType_ = actuatorType;

  // set I/O pointers
  output_ = outputPointer;
  input_  = inputPointer;
}

// for debug output uncomment below line and comment out line after
// void PID::autoTune(Serial *pc, bool PI, PID::t_AutoTuneConfig *autoTuneConfig) {
void PID::autoTune(bool PI, PID::t_AutoTuneConfig *autoTuneConfig) {
  Timer timer;
  bool isMax, isMin, justchanged;
  double absMax, absMin, refVal, Ku, Pu;
  std::chrono::duration<double> peak1, peak2;
  int peakType, peakCount;
  double lastInputs[101] = {0};
  double peaks[10];

  // if no config struct was passed, create one with default values
  // outputStart = half of range, oStep = 10% of range, noiseBand = 1% of range
  if (autoTuneConfig == nullptr) {
    autoTuneConfig             = new PID::t_AutoTuneConfig();
    autoTuneConfig->nLookBack  = 40;
    autoTuneConfig->sampleTime = 250ms;
    double outputStart         = outMax_ / 2;
    if (outputStart < outMin_) outputStart = outMin_;
    autoTuneConfig->outputStart = outputStart;
    autoTuneConfig->oStep       = outSpan_ * 0.15;
    autoTuneConfig->noiseBand   = outSpan_ * 0.01;
    autoTuneConfig->setpoint    = setPoint_;
  }

  // initialize variables
  peakType = peakCount = 0;
  peak1 = peak2 = 0s;
  justchanged   = false;
  refVal = absMax = absMin = autoTuneConfig->setpoint;

  setOutput(autoTuneConfig->outputStart + autoTuneConfig->oStep);

  timer.start();

  while (1) {
    // for debugging uncomment this line and add serial object as argument (called pc)
    // pc->printf("Peak count: %d, \t refVal: %f \r\n", peakCount, refVal);
    // enough peaks to calculate params
    if (peakCount > 9) {
      break;
    }

    std::chrono::duration<double> last_timer_time = timer.elapsed_time();

    refVal = *input_;

    if (refVal > absMax && refVal < inMax_) absMax = refVal;
    if (refVal < absMin && refVal < inMin_) absMin = refVal;

    // oscillate output
    if (refVal > autoTuneConfig->setpoint + autoTuneConfig->noiseBand) {
      setOutput(autoTuneConfig->outputStart - autoTuneConfig->oStep);
    } else if (refVal < autoTuneConfig->setpoint - autoTuneConfig->noiseBand) {
      setOutput(autoTuneConfig->outputStart + autoTuneConfig->oStep);
    }

    isMax = isMin = true;

    // id peaks
    for (int i = autoTuneConfig->nLookBack - 1; i >= 0; i--) {
      double val = lastInputs[i];
      if (isMax) isMax = refVal > val;
      if (isMin) isMin = refVal < val;
      lastInputs[i + 1] = lastInputs[i];
    }
    lastInputs[0] = refVal;

    if (isMax) {
      if (peakType == 0) peakType = 1;
      if (peakType == -1) {
        peakType    = 1;
        justchanged = true;
        peak2       = peak1;
      }
      peak1            = last_timer_time;
      peaks[peakCount] = refVal;

    } else if (isMin) {
      if (peakType == 0) peakType = -1;
      if (peakType == 1) {
        peakType = -1;
        peakCount++;
        justchanged = true;
      }

      if (peakCount < 10) peaks[peakCount] = refVal;
    }

    // Transition, check if we can autotune based on the last peaks
    if (justchanged && peakCount > 2) {
      double avgSeparation =
          (abs(peaks[peakCount - 1] - peaks[peakCount - 2]) + abs(peaks[peakCount - 2] - peaks[peakCount - 3])) / 2;
      if (avgSeparation < 0.05 * (absMax - absMin)) break;
    }
    justchanged = false;

    // wait for sample time interval
    while ((timer.elapsed_time() - last_timer_time) < autoTuneConfig->sampleTime)
      ;
  }

  // calculate autotune parameters TODO: FIXME! Clean this up. Where are these magic numbers from(@tandronescu)?
  Ku = 4 * (2 * autoTuneConfig->oStep) / ((absMax - absMin) * M_PI);
  Pu = (peak1 - peak2).count();

  // if we only PI control is desired, alternate calculations and TauD set to 0
  if (PI) {
    autoTuneKc_   = 0.45 * Ku;
    autoTuneTauR_ = 0.54 * Ku / Pu;
    autoTuneTauD_ = 0;
  } else {
    autoTuneKc_   = 0.6 * Ku;
    autoTuneTauR_ = 1.2 * Ku / Pu;
    autoTuneTauD_ = 0.075 * Ku * Pu;
  }
}

void PID::setOutput(double output) {
  // bound output to limits
  if (output > outMax_)
    output = outMax_;
  else if (output < outMin_)
    output = outMin_;

  // cast void pointer based on required output type
  // add desired output types here and convert from double as necessary
  switch (actuatorType_) {
    case PWM_MOTOR: {
      *static_cast<PwmOut *>(output_) = output;
      break;
    }
    default:
      *(double *)output_ = output;
  }
}

void PID::setAutoTuneParams() {
  setTunings(autoTuneKc_, autoTuneTauR_, autoTuneTauD_);
}

double PID::compute() {
  // Pull in the input and setpoint, and scale them into percent span.
  double scaledPV = (processVariable_ - inMin_) / inSpan_;

  if (scaledPV > 1.0) {
    scaledPV = 1.0;
  } else if (scaledPV < 0.0) {
    scaledPV = 0.0;
  }

  double scaledSP = (setPoint_ - inMin_) / inSpan_;
  if (scaledSP > 1.0) {
    scaledSP = 1;
  } else if (scaledSP < 0.0) {
    scaledSP = 0;
  }

  double error = scaledSP - scaledPV;

  if (fabs(error) < deadZoneError_) {
    error = 0;
  }

  // Check and see if the output is pegged at a limit and only
  // integrate if it is not. This is to prevent reset-windup.
  if (!(prevControllerOutput_ >= 1 && error > 0) && !(prevControllerOutput_ <= 0 && error < 0)) {
    accError_ += error;
  }

  // Compute the current slope of the input signal.
  double dMeas = (scaledPV - prevProcessVariable_) / tSample_.count();

  double scaledBias = 0.0;

  if (usingFeedForward) {
    scaledBias = (bias_ - outMin_) / outSpan_;
  }

  // Perform the PID calculation.
  controllerOutput_ = scaledBias + Kc_ * (error + (tauR_ * accError_) - (tauD_ * dMeas));

  // Make sure the computed output is within output constraints.
  if (controllerOutput_ < 0.0) {
    controllerOutput_ = 0.0;
  } else if (controllerOutput_ > 1.0) {
    controllerOutput_ = 1.0;
  }

  // Remember this output for the windup check next time.
  prevControllerOutput_ = controllerOutput_;
  // Remember the input for the derivative calculation next time.
  prevProcessVariable_ = scaledPV;

  // Scale the output from percent span back out to a real world number.
  return ((controllerOutput_ * outSpan_) + outMin_);
}

float PID::getInMin() const {
  return inMin_;
}

float PID::getInMax() const {
  return inMax_;
}

float PID::getOutMin() const {
  return outMin_;
}

float PID::getOutMax() const {
  return outMax_;
}

std::chrono::duration<double> PID::getInterval() const {
  return tSample_;
}

float PID::getPParam() const {
  return pParam_;
}

float PID::getIParam() const {
  return iParam_;
}

float PID::getDParam() const {
  return dParam_;
}

float PID::getBiasParam() const {
  return bias_;
}

float PID::getDeadzoneParam() const {
  return deadZoneError_;
}

float PID::getATunePParam() const {
  return autoTuneKc_;
}

float PID::getATuneIParam() const {
  return autoTuneTauR_;
}

float PID::getATuneDParam() const {
  return autoTuneTauD_;
}

float PID::getSetPoint() const {
  return setPoint_;
}
