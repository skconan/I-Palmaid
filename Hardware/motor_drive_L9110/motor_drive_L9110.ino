
/*
 Stepper Motor Control - one revolution

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor should revolve one revolution in one direction, then
 one revolution in the other direction.


 Created 11 Mar. 2007
 Modified 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>

const int stepsPerRevolution = 250;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
//Forward
// 7 > B1-A, 3 > B1-B, 9 > A1-A, 8 > A1-B
Stepper forward(stepsPerRevolution, 7,3, 9,8);
//Backward
Stepper backward(stepsPerRevolution, 3,7, 8,9);
//Right
Stepper left(stepsPerRevolution, 7,3, 8,9);
//Left
Stepper left(stepsPerRevolution, 3,7, 9,8);

void setup() {
  // set the speed at 60 rpm:
  forward.setSpeed(60);
  backward.setSpeed(60);
  left.setSpeed(60);
//myStepper1.setSpeed(60);
  // initialize the serial port:
  Serial.begin(115200);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("Forward");
  forward.step(stepsPerRevolution);
  delay(1500);

  // step one revolution in the other direction:
  Serial.println("Backward");
  backward.step(stepsPerRevolution);
  delay(1500);

  Serial.println("Left");
  left.step(stepsPerRevolution);
  delay(1500);
}

