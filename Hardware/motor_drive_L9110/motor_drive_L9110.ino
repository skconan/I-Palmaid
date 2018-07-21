
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

const int steps = 48;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
//Forward
// 7 > B1-A, 3 > B1-B, 9 > A1-A, 8 > A1-B
Stepper forward(steps, 3,7, 9,8);
//Backward
//Stepper backward(stepsPerRevolution, 3,7, 8,9);
////Right
//Stepper left(stepsPerRevolution, 7,3, 8,9);
////Left
//Stepper left(stepsPerRevolution, 3,7, 9,8);

void set_speed_motor(){
//  motor speed in rotations per minute (RPMs)
  uint32_t rpm = 200;
  forward.setSpeed(rpm);
//  backward.setSpeed(rpm);
//  left.setSpeed(rpm);
}
void setup() {
  // initialize the serial port:
  Serial.begin(115200);
  set_speed_motor();
}
void robot_forward(){
  Serial.println("Forward");
  forward.step(steps);
  Serial.println("End Step");
  delay(600);
  Serial.println("End Forward");
}
void loop() {
  // step one revolution  in one direction:

  robot_forward();


}

