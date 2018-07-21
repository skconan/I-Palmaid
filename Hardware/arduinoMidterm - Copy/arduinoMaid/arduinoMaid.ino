#include <SoftwareSerial.h>
#include <Servo.h>
#include <Stepper.h>
#include "DHT.h"

#define SW 2
#define BUZZER 3
#define LED_Y 4
#define servo 7
#define DHTPIN A1
#define DHTTYPE DHT22
#define ldr A0
#define trigger_pin 5
#define in_pin 6

Servo myservo;
SoftwareSerial se_read(12, 13); // write only
SoftwareSerial se_write(10, 11); // read only

const int stepsPerRevolution = 250;
//Forward
Stepper forward(stepsPerRevolution, 7,3, 9,8);
//Backward
Stepper backward(stepsPerRevolution, 3,7, 8,9);
//Right
Stepper right(stepsPerRevolution, 7,3, 8,9);
//Left
Stepper left(stepsPerRevolution, 3,7, 9,8);

//Stepper motorstop(1, 3, 7, 9, 8);
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
} project_data = { 0, 0, 0 , 0, 0};

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

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  pinMode(SW, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(ldr, INPUT);
  pinMode(trigger_pin, OUTPUT);
  pinMode(in_pin, INPUT);
  myservo.attach(servo);
  myservo.write(0);
  dht.begin();
  //////<--Motor-->///////
  forward.setSpeed(60);
  backward.setSpeed(60);
  left.setSpeed(60);
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
uint32_t lastRobotTime = 0;
uint32_t last_sent_time = 0;
boolean is_data_header = false;
char expected_data_size = 0;
char cur_data_header = 0;
char buffer[256];
int8_t cur_buffer_length = -1;
int32_t b = -1;

void loop() {
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
//  Serial.print("SW Send to Node: ");
//  Serial.println(project_data.sw);
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
    Serial.println("SEND");
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
            
            //Serial.println(server_data.switchStatus);
//            project_data.motor = data -> motorStatus;//////////////Read from Server

//            if(project_data.sw == 0) {
//              project_data.motor = 1;
//            }
            if(data -> motorStatus == 1){
              Serial.println("Forward");
               forward.step(stepsPerRevolution);
//              if(project_data.curRoom < data -> goRoom) {
//                //Motor Forward
//              }
//              else if(project_data.curRoom > data -> goRoom) {
//                //Motor Back
//              }
//
//              else { //curRoom == goRoom
//                //Motor off
//                project_data.motor = 0;
//              }
              
            }
            else if(data -> motorStatus == 0) {
              //Motor Off
                Serial.println("motorstop");
                backward.step(stepsPerRevolution);
            }
//            if(data -> switchStatus == 0) {
//              digitalWrite(LED_Y, HIGH);
//            }
//            else if(data -> switchStatus != 0) {
//              digitalWrite(LED_Y, LOW);
//            }

//            if(project_data.light < 300 and project_data.motor == 1) {  //sensors check robot walk throught
//              uint32_t curShadowOnLdr = millis();
//              if(curShadowOnLdr - lastRobotTime > 2000) { //ทับแถบดำนาน 1
//                Serial.print("<+++++++++++++++On black++++++++++++++>");
//                lastRobotTime = curShadowOnLdr;
//              }
//              else {
//                Serial.print("<-------CurRoom + 1 ------->");
//                project_data.curRoom += 1;
//                uint32_t curShadowOnLdr = 0;
//                project_data.motor = 0;
//              }
//            }
                        
            if(project_data.ultrasonic < 10 and data -> motorStatus == 1) {
              //STOP MOTOR
              Serial.println("Ultrasonic Enable");
              project_data.motor = 0;
            }
            //server_data.plus = data->plus;
//            send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ProjectData));
            Serial.println("UPDATE");
          } break;
        }
        cur_buffer_length = -1;
      }
    }
  }
}

