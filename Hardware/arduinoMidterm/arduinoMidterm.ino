#include <SoftwareSerial.h>
#include <Servo.h>
#include "DHT.h"
#define SW 2
#define BUZZER 3
#define LED_Y 4
#define LED_B 5
#define LED_W 6
#define servo 7
#define DHTPIN A1
#define DHTTYPE DHT22
#define ldr A0

Servo myservo;
SoftwareSerial se_read(12, 13); // write only
SoftwareSerial se_write(10, 11); // read only
///////////////////////////////
///////////////////////////////
struct ProjectData {
  int32_t sw;
  int32_t led_Y;
  int32_t led_B;
  int32_t led_W;
  float humid;
  int32_t light;
} project_data = { -1, -1, -1 , -1, -1, -1};

struct ServerData {
  int32_t swPressed;
  int32_t airOn;
  int32_t lightIn;
  int32_t lightOut;
  int32_t doorOn;
} server_data = { -1, -1, -1, -1, -1};

const char GET_SERVER_DATA = 1;
const char GET_SERVER_DATA_RESULT = 2;
const char UPDATE_PROJECT_DATA = 3;

void ringOn(){
  tone(BUZZER, 2000);
  delay(500);
  noTone(BUZZER);
  delay(500);
  tone(BUZZER, 2000);
  delay(200);
  noTone(BUZZER);
  delay(200);
  tone(BUZZER, 2000);
  delay(200);
  noTone(BUZZER);
  delay(200);
  tone(BUZZER, 2000);
  delay(200);
  noTone(BUZZER);
}
void playNote(char note) {
  char names[] = { ' ', 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
  // play the tone corresponding to the note name
  if(note != ' '){
    tone(BUZZER, tone[note]);
    delay(200);
    noTone(BUZZER);
  }
  else {
    delay(200);
  }
}

void playSong(){
    playNote(1);
    playNote(1);
    playNote(2);
    playNote(2);
    playNote(6);
    playNote(6);
    playNote(5);
    playNote(0);
    playNote(4);
    playNote(4);
    playNote(2);
    playNote(2);
    playNote(3);
    playNote(3);
    playNote(1);
  
}

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
  pinMode(LED_B, OUTPUT);
  pinMode(LED_W, OUTPUT);
  pinMode(ldr, INPUT);
  myservo.attach(servo);
  myservo.write(0);
  dht.begin();
  
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

uint32_t last_sent_time = 0;
boolean is_data_header = false;
char expected_data_size = 0;
char cur_data_header = 0;
char buffer[256];
int8_t cur_buffer_length = -1;
int32_t b = -1;
void loop() {
  delay(500);
  uint32_t cur_time = millis();
  //read from sensor....
  int a = digitalRead(SW);
  project_data.humid = dht.readHumidity();
  project_data.sw = a; //set sw to project data
  Serial.print("SW Input: ");
  Serial.println(project_data.sw);
  project_data.light = analogRead(ldr);
  Serial.print("light :");
  Serial.print(project_data.light);
  Serial.print(" Humid :");
  Serial.println(project_data.humid);
//  project
  if(a == 0){
    Serial.println("Press");
    delay(500);
  }
  //Serial.println(server_data.airOn);
  /*project_data.plus = b;
  if(server_data.plus == 1){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }*/
  
  if (cur_time - last_sent_time > 500) {//always update
//    Serial.println(project_data.plus);
    send_to_nodemcu(GET_SERVER_DATA, &server_data, sizeof(ServerData));
    send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ServerData));
    last_sent_time = cur_time;
  }
  //send to nodemcu
  
  //read data from server pass by nodemcu
  while (se_read.available()) {
    char ch = se_read.read();
    Serial.print("RECV: ");
    Serial.println((byte)ch);
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
            Serial.print("SW: ");
            Serial.println(data->swPressed);
            //Serial.println(server_data.swPressed);
            if(project_data.sw == 0) {
              //playSong();
              /*tone(BUZZER, 2000);
              delay(500);
              noTone(BUZZER);
              Serial.print("Buzzer: ON ");
              */
              ringOn();
            }
            else if(project_data.sw == 1) {
              Serial.print("Buzzer: OFF ");
            }
            
            if(data -> doorOn == 0) {
              myservo.write(180);
              //delay(500);
              //project_data.servo = 1;
            }
            
            else if(data -> doorOn == 1) {
              myservo.write(0);
              //project_data.servo = 0;
              
            }
              /*tone(buzzer, 2000);
              delay(200);
              noTone(buzzer);
              tone(buzzer, 2000);
              delay(200);
              noTone(buzzer);*/
            if(data -> airOn == 1 or project_data.humid > 700) {
              digitalWrite(LED_Y, HIGH);
              Serial.print("Air: ON ");
              project_data.led_Y = 1;
              //delay(500);
            }
            else if(data -> airOn == 0 or project_data.humid < 700) {
              digitalWrite(LED_Y, LOW);
              Serial.print("Air: OFF ");
              project_data.led_Y = 0;
              //delay(500);
            }
            
            if(data -> lightOut == 1 or project_data.light <= 200) {
              digitalWrite(LED_B, HIGH);
              Serial.print("LightOut: ON ");
              project_data.led_B = 1;
              //delay(500);
            }
            else if(data -> lightOut == 0 or project_data.light > 200) {
              digitalWrite(LED_B, LOW);
              Serial.print("LightOut: OFF ");
              project_data.led_B = 0;
              //delay(500);
            }

            if(data -> lightIn == 1) {
              digitalWrite(LED_W, HIGH);
              Serial.print("LightIn: ON ");
              project_data.led_W = 1;
              //delay(500);
            }
            else if(data -> lightIn == 0) {
              digitalWrite(LED_W, LOW);
              Serial.println("LightIn: OFF ");
              project_data.led_W = 0;
              //delay(500);
            }
            
            
            /*Serial.print("temp status: ");
            Serial.println(data->temp);
            Serial.print("light: ");
            Serial.println(data->light_lux);
            Serial.print("sound status: ");
            Serial.println(data->sound);
            Serial.print("door: ");
            Serial.println(data->door);
            Serial.print("PLUS: ");
            Serial.println(data->plus);*/
            /*if(data->door == 1) {
              digitalWrite(LED_BUILTIN, HIGH);
            } else {
              digitalWrite(LED_BUILTIN, LOW);
            }*/
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

