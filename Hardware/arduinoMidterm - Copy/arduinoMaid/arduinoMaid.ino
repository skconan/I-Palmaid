#include <SoftwareSerial.h>
#include <Servo.h>
#include <Stepper.h>
#include "DHT.h"

#define SW 2
#define ldr A0
#define trigger_pin 6
#define in_pin 5

Servo myservo;
SoftwareSerial se_read(12, 13); // write only
SoftwareSerial se_write(10, 11); // read only

///////////////////////////////////////////////////////////
const int steps = 48;
//Forward
Stepper forward(steps, 3,7, 8,9);
//Backward L H L H
Stepper backward(steps, 7,3, 9,8);
//Right
Stepper right(steps, 7,3, 8,9);
//Left
Stepper left(steps, 3,7, 9,8);

//Stepper motorstop(1, 3, 7, 9, 8);
void set_speed_motor(){
//  motor speed in rotations per minute (RPMs)
  uint32_t rpm = 200;
  forward.setSpeed(rpm);
  backward.setSpeed(rpm);
  right.setSpeed(rpm);
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
  delay(500);
}
//////////////////////////////////////////////////
long duration, cm;

long microsecondsToCentimeters( long microseconds)
{
  return microseconds / 29 / 2;
}


///////////////////////////////
///////////////////////////////
struct ProjectData {
  int32_t sw;
  int32_t light;
  int32_t motor;
  int32_t ultrasonic;
  int32_t curRoom;
//  int32_t isRobotOnSensors;
//  int32_t isRobotOnRoom;
} project_data = { 1, 0, 0, 0, 0};
int32_t motor = 0;
struct ServerData {
  int32_t switchStatus;
  int32_t motorStatus;
  int32_t goRoom;
} server_data = { 0, 0, 0};

const char GET_SERVER_DATA = 1;
const char GET_SERVER_DATA_RESULT = 2;
const char UPDATE_PROJECT_DATA = 3;

void send_to_nodemcu(char code, void *data, char data_size) {
  char *b = (char*)data; 
  char sent_size = 0;
  while (se_write.write(code) == 0) {
    delay(1);
    
  }
  while (sent_size < data_size) {
    sent_size += se_write.write(b, data_size);
    delay(1);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(SW, INPUT);
  pinMode(trigger_pin, OUTPUT);
  pinMode(in_pin, INPUT);
  //////<--Motor-->///////
    set_speed_motor();
//  forward.setSpeed(60);
//  backward.setSpeed(60);
//  left.setSpeed(60);
  ///////////////////////
  Serial.begin(115200);
  se_read.begin(38400);
  se_write.begin(38400);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!se_read.isListening()) {
    se_read.listen();
  }
  //pinMode(SW,INPUT_PULLUP);
  Serial.println((int)sizeof(ServerData));
  Serial.println("ARDUINO READY!");
}
uint32_t lastTime = 0;
uint32_t lastRobotTime = 0;
uint32_t last_sent_time = 0;
boolean is_data_header = false;
char expected_data_size = 0;
char cur_data_header = 0;
char buffer[256];
int8_t cur_buffer_length = -1;
int32_t b = -1;

void loop() {
  project_data.motor = motor;
//  Serial.println("LOOP");
  delay(500);
  uint32_t cur_time = millis();
  //read from sensor....
  Serial.println("\n############## Project Data ################");
  int a = digitalRead(SW); /////////SWITCH
  ///////////////////////////////////////// Ultrasonic
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  duration = pulseIn(in_pin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.print("Ultrasonic: ");
  Serial.print(cm);
  Serial.println(" cm.");
  ////////////////////////////////////////
  project_data.sw = a; //set sw to project data
  Serial.print("SW Send to Node: ");
  Serial.println(project_data.sw);
  project_data.light = analogRead(ldr); ////Light
  Serial.print("light :");
  Serial.println(project_data.light);
//  project
  if(a == 0){
    Serial.println("Press");
    delay(500);
  }
  Serial.println("###################################\n");
  delay(1000);
  //Serial.println(server_data.airOn);
  /*project_data.plus = b;
  if(server_data.plus == 1){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }*/
//  Serial.print("SizeS: ");
//  Serial.println(sizeof(ServerData));
//  Serial.print("SizeP: ");
//  Serial.println(sizeof(ProjectData));
  if (cur_time - last_sent_time > 500) {//always update
//    Serial.println(project_data.plus);
    send_to_nodemcu(GET_SERVER_DATA, &server_data, sizeof(ServerData));
    send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ProjectData));
    Serial.println("SEND TO NODE");
    Serial.println(project_data.motor);
    last_sent_time = cur_time;
    
  }
  //send to nodemcu
//  Serial.println("Start Reading");
  //read data from server pass by nodemcu
  while (se_read.available()) {
    char ch = se_read.read();
//    Serial.println("Reading success!!!!!!!!!!!");
    //Serial.println("Serial read");
//    Serial.print("RECV: ");
//    Serial.println((byte)ch);
    if (cur_buffer_length == -1) {
      cur_data_header = ch;
      switch (cur_data_header) {
        case GET_SERVER_DATA_RESULT:
        //unknown header
          expected_data_size = sizeof(ServerData);
          cur_buffer_length = 0;
          break;
      }
    } else if (cur_buffer_length < expected_data_size) {
      buffer[cur_buffer_length++] = ch;
      if (cur_buffer_length == expected_data_size) {
        switch (cur_data_header) {
          case GET_SERVER_DATA_RESULT: {
            ServerData *data = (ServerData*)buffer;
            //use data to control sensor
            Serial.println("<-------Server Data-------->");
//            Serial.print("SW from Node: ");
//            Serial.println(data->switchStatus);
            Serial.print("Motor Status: ");
//            Serial.println(project_data.sw);
//            Serial.println(project_data.motor);
            Serial.println(data -> motorStatus);
            Serial.print("curRoom --> goRoom: ");
            Serial.print(project_data.curRoom);
            Serial.print(" ---> ");
            Serial.println(data -> goRoom);
            
            project_data.motor = data -> motorStatus;
            //Serial.println(server_data.switchStatus);
//            project_data.motor = data -> motorStatus;//////////////Read from Server

//            if(project_data.sw == 0) {
//              project_data.motor = 1;
//            }
            if(project_data.motor == 1){
//              project_data.motor = 1;
              Serial.println("Forward");
              robot_forward();
              
//              if(project_data.curRoom < data -> goRoom) {
//                //Motor Forward
//                Serial.println("Forward");
//                robot_forward();
//              }
//              else if(project_data.curRoom > data -> goRoom) {
//                //Motor Back
//                Serial.println("backward");
//                robot_backward();
//              }
//
//              else { //curRoom == goRoom
//                //Motor off
//                  Serial.println("motorstop");
//                robot_stop();
//              }
              
            }
            else if(project_data.motor == 0) {
              //Motor Off
                robot_stop();
                Serial.println("motorstop");
//                delay(200);
//                backward.step(stepsPerRevolution);
            }

//            if(project_data.light < 300 and project_data.motor == 1) {  //sensors check robot walk throught
//              uint32_t curShadowOnLdr = millis();
//              if(curShadowOnLdr - lastRobotTime > 2000) { //ทับแถบดำนาน 2 วิ
//                Serial.print("<+++++++++++++++On black++++++++++++++>");
//                lastRobotTime = curShadowOnLdr;
//              }
//              else {
//                Serial.print("<-------CurRoom Changed ------->");
//                if(project_data.curRoom == 0) project_data.curRoom = 1;
//                else project_data.curRoom = 0;
//                if(project_data.curRoom < data -> goRoom) project_data.curRoom += 1
//                else if(project_data.curRoom > data -> goRoom) project_data.curRoom -= 1
//                uint32_t curShadowOnLdr = 0;
//                project_data.motor = 0;
//              }
//            }
                        
            if(project_data.ultrasonic < 10 and project_data.motor == 1) {
              //STOP MOTOR
              Serial.println("Ultrasonic Enable!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
              project_data.motor = 0;
            }
            else if(project_data.ultrasonic > 10 and project_data.motor == 0) {
              //STOP MOTOR
              Serial.println("Ultrasonic Unable");
              project_data.motor = 1;
            }
//            uint32_t curDataTime = millis();
//            if (curDataTime - lastTime > 500) {
//                send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ProjectData));
//                lastTime = curDataTime;
//              }
//              send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ProjectData));
//            }
            
//            send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ProjectData));
            Serial.println("UPDATE");
          } break;
        }
        cur_buffer_length = -1;
      }
    }
  }
  motor = project_data.motor;
}

