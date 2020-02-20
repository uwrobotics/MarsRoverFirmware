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
 */

#ifndef PID_H
#define PID_H

/**
 * Includes
 */
#include "mbed.h"

/**
 * Defines
 */
#define PID_MANUAL_MODE 0
#define PID_AUTO_MODE 1

/**
 * Proportional-integral-derivative controller.
 */
class PID
{

public:
    typedef struct
    {
        float P, I, D, bias = 0.0;
        float deadZoneError = 0.0;
    } t_pidConfig;

    // add other actuators here
    enum ActuatorType
    {
        PWM_MOTOR
    };

    // configurable parameters for autotuning process
    typedef struct
    {
        int nLookBack;
        unsigned long sampleTime;
        double outputStart, oStep, noiseBand, setpoint;
    } t_AutoTuneConfig;

    /**
     * Constructor.
     *
     * Sets default limits [0-3.3V], calculates tuning parameters, and sets
     * manual mode with no bias.
     *
     * @param Kc - Tuning parameter
     * @param tauI - Tuning parameter
     * @param tauD - Tuning parameter
     * @param interval PID calculation performed every interval seconds.
     */
    PID(float Kc, float tauI, float tauD, float interval);

    /**
     * Scale from inputs to 0-100%.
     *
     * @param InMin The real world value corresponding to 0%.
     * @param InMax The real world value corresponding to 100%.
     */
    void setInputLimits(float inMin, float inMax);

    /**
     * Scale from outputs to 0-100%.
     *
     * @param outMin The real world value corresponding to 0%.
     * @param outMax The real world value corresponding to 100%.
     */
    void setOutputLimits(float outMin, float outMax);

    /**
     * Calculate PID constants.
     *
     * Allows parameters to be changed on the fly without ruining calculations.
     *
     * @param Kc - Tuning parameter
     * @param tauI - Tuning parameter
     * @param tauD - Tuning parameter
     */
    void setTunings(float Kc, float tauI, float tauD);

    /**
     * Reinitializes controller internals. Automatically
     * called on a manual to auto transition.
     */
    void reset(void);

    /**
     * Set PID to manual or auto mode.
     *
     * @param mode        0 -> Manual
     *             Non-zero -> Auto
     */
    void setMode(int mode);

    /**
     * Set how fast the PID loop is run.
     *
     * @param interval PID calculation peformed every interval seconds.
     */
    void setInterval(float interval);

    /**
     * Set the set point.
     *
     * @param sp The set point as a real world value.
     */
    void setSetPoint(float sp);

    /**
     * Set the process value.
     *
     * @param pv The process value as a real world value.
     */
    void setProcessValue(float pv);

    /**
     * Set the bias.
     *
     * @param bias The bias for the controller output.
     */
    void setBias(float bias);

    /**
     * Set the dead zone error to allow error to round down if within +/- this value
     * @param error Round error down to 0.0 if error is within +/- this value
     */
    void setDeadZoneError(float error);

    /**
     * Set real output value
     * @param realOutput real life output
     */
    void setRealOutput(float realOutput);

    /**
     * Setup needed before autotuning
     * @param outputPointer generic pointer to actual output (ex. PWM pin)
     * @param inputPointer pointer to calculated input value, usually done in background on main
     * @param actuatorType type of actuator needed to cast generic void pointer 
     */
    void setupAutoTune(void *outputPointer, float *inputPointer, int actuatorType);

    /**
     * Run the autotuning algorithm and set autoTune class member variables
     * @param PI choose between PI and PID control, calculation is different
     * @param autoTuneConfig pointer to config struct for autotune, optional
     */
    // uncomment below line and comment line after for debug output
    // void autoTune(Serial *pc, bool PI, t_AutoTuneConfig *autoTuneConfig = nullptr);
    void autoTune(bool PI, t_AutoTuneConfig *autoTuneConfig = nullptr);

    /**
     * Set the output from within the PID class, needed for autotuning
     * @param output output as a float, casting done in function based on actuator type
     */
    void setOutput(float output);

    /**
     * Set the tuning values to the calculated autotuning parameters
     */
    void setAutoTuneParams();

    /**
     * PID calculation.
     *
     * @return The controller output as a float between outMin and outMax.
     */
    float compute(void);

    //Getters.
    float getInMin();
    float getInMax();
    float getOutMin();
    float getOutMax();
    float getInterval();
    float getPParam();
    float getIParam();
    float getDParam();
    float getATunePParam();
    float getATuneIParam();
    float getATuneDParam();
    float getSetPoint();

private:
    bool usingFeedForward;
    bool inAuto;

    //Actual tuning parameters used in PID calculation.
    float Kc_;
    float tauR_;
    float tauD_;

    //Tuning parameters calculated from autotuning function
    float autoTuneKc_;
    float autoTuneTauR_;
    float autoTuneTauD_;

    //Input/Output pointers for AutoTuning
    void *output_;
    float *input_;

    //Store type of actuator PID is being used on
    int actuatorType_;

    //Struct with all the autotune parameters
    t_AutoTuneConfig AutoTuneConfig;

    //Raw tuning parameters.
    float pParam_;
    float iParam_;
    float dParam_;

    //The point we want to reach.
    float setPoint_;
    //The thing we measure.
    float processVariable_;
    float prevProcessVariable_;
    //The output that affects the process variable.
    float controllerOutput_;
    float prevControllerOutput_;

    //We work in % for calculations so these will scale from
    //real world values to 0-100% and back again.
    float inMin_;
    float inMax_;
    float inSpan_;
    float outMin_;
    float outMax_;
    float outSpan_;

    //The accumulated error, i.e. integral.
    float accError_;
    //The allowed error range for error to be rounded to 0.0
    float deadZoneError_;
    //The controller output bias.
    float bias_;

    //The interval between samples.
    float tSample_;

    //Controller output as a real world value.
    volatile float realOutput_;
};

#endif /* PID_H */
