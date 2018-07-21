#include <SoftwareSerial.h>
#define SW 8
#define LED 9

SoftwareSerial se_read(12, 13); // write only
SoftwareSerial se_write(10, 11); // read only

struct ProjectData {
  /*your data*/
  float temperatature;
  int32_t LED;
  int32_t SW;
  
} project_data = {25.25, 0, 800}; //your value

struct ServerData {
  /*your data*/
  int32_t is_lamp_on;
  int32_t is_button_pressed;
} server_data = {0, 0}; // your value

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
  Serial.begin(115200);
  se_read.begin(38400);
  se_write.begin(38400);
  
  ///////////////////////
  pinMode(LED, OUTPUT); 
  pinMode(SW, INPUT);
  ///////////////////////
  
  pinMode(LED_BUILTIN, OUTPUT);
  while (!se_read.isListening()) {
    se_read.listen();
  }

  Serial.println((int)sizeof(ServerData));
  Serial.println("ARDUINO READY!");
}

uint32_t last_sent_time = 0;
boolean is_data_header = false;
char expected_data_size = 0;
char cur_data_header = 0;
char buffer[256];
int8_t cur_buffer_length = -1;
void loop() {
  uint32_t cur_time = millis();
  //send to nodemcu
  if (cur_time - last_sent_time > 500) {//always update
    send_to_nodemcu(UPDATE_PROJECT_DATA, &project_data, sizeof(ProjectData));
    send_to_nodemcu(GET_SERVER_DATA, &server_data, sizeof(ServerData));
    last_sent_time = cur_time;
  }

  //read from sensor....
  project_data.SW = digitalRead(SW);
  //send to nodemcu
  
  //read data from server pass by nodemcu
  while (se_read.available()) {
    char ch = se_read.read();
    //Serial.print("RECV: ");
    //Serial.println((byte)ch);
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
            if(server_data -> is_button_pressed == 0){
              digitalWrite(LED, HIGH);
            }
            else{
              digitalWrite(LED, LOW);
            }
          } break;
        }
        cur_buffer_length = -1;
      }
    }
  }
}

