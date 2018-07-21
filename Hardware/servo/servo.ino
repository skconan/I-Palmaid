#include <Servo.h>
#define servo 7
#define sw 2
Servo myservo;
//red power, brown ground, yellow port
int last = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sw, INPUT);
  myservo.attach(servo);
  myservo.write(0);
}
int pressed;

void loop() {
  // put your main code here, to run repeatedly:
  pressed = digitalRead(sw);
  Serial.println(last);
  if(pressed == 0) {
    myservo.write(90);
    delay(1000);
    last == 1;
  }
  
  else{
    myservo.write(0);
    delay(1000);
    last = 0 ;
    Serial.println(last);
  }
  
    
  //myservo.write(0); 
  //delay(1000);
}
