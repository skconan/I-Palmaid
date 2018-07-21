
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
Stepper forward(steps, 3,7, 8,9);
//Backward L H L H
Stepper backward(steps, 7,3, 9,8);
//Right
Stepper right(steps, 7,3, 8,9);
//Left
Stepper left(steps, 3,7, 9,8);

void set_speed_motor(){
//  motor speed in rotations per minute (RPMs)
  uint32_t rpm = 200;
  forward.setSpeed(rpm);
  backward.setSpeed(rpm);
  right.setSpeed(rpm);
}
void setup() {
  // initialize the serial port:
  Serial.begin(115200);
  set_speed_motor();
}
void robot_forward(){
  Serial.println("Forward");
  forward.step(steps);
  delay(500);
  Serial.println("End Forward");
}
void robot_backward(){
  Serial.println("Backward");
  backward.step(steps);
  delay(500);
  Serial.println("End Backward");
}
void robot_right(){
  Serial.println("Right");
  right.step(steps);
  delay(500);
  Serial.println("End Right");
}
void robot_left(){
  Serial.println("Left");
  left.step(steps);
  delay(500);
  Serial.println("End Left");
}
void robot_stop(){
  Serial.println("Robot Stop");
digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(3,LOW);
    digitalWrite(7,HIGH);
  delay(100);

}
void loop() {
  // step one revolution  in one direction:
  
  robot_forward();
  delay(2000);
  
  robot_backward();
//  
  delay(2000);
   robot_right();
//  
  delay(2000);
  robot_left();
//  
  delay(2000);
}

