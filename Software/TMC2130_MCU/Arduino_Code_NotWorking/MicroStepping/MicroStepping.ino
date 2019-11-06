/*
 * Microstepping demo
 *
 * This requires that microstep control pins be connected in addition to STEP,DIR
 *
 * Copyright (C)2015 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 200
#define DEG 1080
#define DIR_1 44
#define STEP_1 42
#define DIR_2 48
#define STEP_2 46
#define DIR_3 30
#define STEP_3 32
#define DIR_4 28
#define STEP_4 26
#define SLEEP 13 // optional (just delete SLEEP from everywhere if not used)

/*
 * Choose one of the sections below that match your board
 */

//#include "DRV8834.h"
//#define M0 10
//#define M1 11
//DRV8834 stepper(MOTOR_STEPS, DIR, STEP, SLEEP, M0, M1);

// #include "A4988.h"
// #define MS1 10
// #define MS2 11
// #define MS3 12
// A4988 stepper(MOTOR_STEPS, DIR, STEP, SLEEP, MS1, MS2, MS3);

 #include "DRV8825.h"
 #define MODE0 51
 #define MODE1 52
 #define MODE2 12
 DRV8825 stepper_1(MOTOR_STEPS, DIR_1, STEP_1, SLEEP, MODE0, MODE1, MODE2);
 DRV8825 stepper_2(MOTOR_STEPS, DIR_2, STEP_2, SLEEP, MODE0, MODE1, MODE2);
 DRV8825 stepper_3(MOTOR_STEPS, DIR_3, STEP_3, SLEEP, MODE0, MODE1, MODE2);
 DRV8825 stepper_4(MOTOR_STEPS, DIR_4, STEP_4, SLEEP, MODE0, MODE1, MODE2);


// #include "DRV8880.h"
// #define M0 10
// #define M1 11
// #define TRQ0 6
// #define TRQ1 7
// DRV8880 stepper(MOTOR_STEPS, DIR, STEP, SLEEP, M0, M1, TRQ0, TRQ1);

// #include "BasicStepperDriver.h" // generic
// BasicStepperDriver stepper(DIR, STEP);

void setup() {
    /*
     * Set target motor RPM.
     */
    stepper_1.begin(RPM);
    stepper_2.begin(RPM);
    stepper_3.begin(RPM);
    stepper_4.begin(RPM);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    // stepper.setEnableActiveState(LOW);
    stepper_1.enable();
    stepper_2.enable();
    stepper_3.enable();
    stepper_4.enable();
    // set current level (for DRV8880 only). 
    // Valid percent values are 25, 50, 75 or 100.
    // stepper.setCurrent(100);
}

void loop() {
    delay(1000);

    /*
     * Moving motor in full step mode is simple:
     */
    stepper_1.setMicrostep(4);   // Set microstep mode to 1:8
    stepper_2.setMicrostep(4); 
    stepper_3.setMicrostep(4); 
    stepper_4.setMicrostep(4); 
    // One complete revolution is still 360° regardless of microstepping mode
    // rotate() is easier to use than move() when no need to land on precise microstep position
    stepper_1.rotate(DEG);    
    stepper_1.rotate(-DEG);
    stepper_2.rotate(DEG);    
    stepper_2.rotate(-DEG);
    stepper_3.rotate(DEG);    
    stepper_3.rotate(-DEG);
    stepper_4.rotate(DEG);    
    stepper_4.rotate(-DEG);
    delay(1000);
}
