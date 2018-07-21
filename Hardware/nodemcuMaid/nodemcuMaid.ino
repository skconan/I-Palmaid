#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <EspSoftwareSerial.h>
#include <math.h>

SoftwareSerial se_read(D5, D6); // write only
//  Arduino D5-11, D6-10
SoftwareSerial se_write(D0, D1); // read only
//  Arduino D0-13, D1-12

//String const url = "http://exceed.srakrn.me/api/";
//String const url = "http://ecourse.cpe.ku.ac.th/exceed/api/";
String const url = "http://ecourse.cpe.ku.ac.th/exceed/api/";

struct ProjectData {
  int32_t sw;
  int32_t light;
  int32_t motor;
  int32_t ultrasonic;
  int32_t curRoom;
//  int32_t isRobotOnSensors;
//  int32_t isRobotOnRoom;
} project_data = { -1, -1, -1, -1, -1,};


struct ServerData {
  int32_t switchStatus;
  int32_t motorStatus;
  int32_t goRoom;
} server_data = { -1,-1,-1,};

const char GET_SERVER_DATA = 1;
const char GET_SERVER_DATA_RESULT = 2;
const char UPDATE_PROJECT_DATA = 3;

const char GET_SERVER_DATA_ACTION = 4;
const char GET_SERVER_DATA_RESULT_ACTION = 5;
const char UPDATE_PROJECT_DATA_ACTION = 6;

// wifi configuration
//const char SSID[] = "Unknow";
//const char PASSWORD[] = "1q2w3e4r";
//const char SSID[] = "KUWIN_QUEEN_2.4GHz";
const char SSID[] = "EXCEED_LEFT_2_2.4GHz";
const char PASSWORD[] = "1234567890";

// for nodemcu communication
uint32_t last_sent_time = 0;
char expected_data_size = 0;
char cur_data_header = 0;
char buffer[256];
int8_t cur_buffer_length = -1;

void send_to_arduino(char code, void *data, char data_size) {
  char *b = (char*)data;
  int sent_size = 0;
  while (se_write.write(code) == 0) {
    delay(1);
  }
  while (sent_size < data_size) {
    sent_size += se_write.write(b, data_size);
    delay(1);
  }
}

void wifi_initialization() {
  Serial.println("WIFI INITIALIZING.");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    yield();
    delay(10);
  }

  Serial.println("WIFI INITIALIZED.");
}

void serial_initialization() {
  Serial.begin(115200);
  se_read.begin(38400);
  se_write.begin(38400);

  while (!se_read.isListening()) {
    se_read.listen();
  }

  Serial.println();
  Serial.println("SERIAL INITIALIZED.");
}

String set_builder(const char *key, int32_t value) {
  String str = url;
  str = str + key;
  str = str + "/set?value=";
  str = str + value;
  Serial.println(str);
  return str;
}

String get_builder(const char *key) {
  String str = url;
  str = str + key;
  str = str + "/view/";
  return str;
}

void update_data_to_server_callback(String const &str) {
  Serial.println("update_data_to_server_callback FINISHED!");
}

bool GET(const char *url, void (*callback)(String const &str,int32_t &value), int32_t &value) {
  HTTPClient main_client;
  main_client.begin(url);
  if (main_client.GET() == HTTP_CODE_OK) {
    Serial.println("GET REQUEST RESPONSE BEGIN");
    if (callback != 0) {
      callback(main_client.getString(),value);
    }
    delay(200);
    return true;
  }
  Serial.println("GET REQUEST RESPONSE BEGIN");
  return false;
}
bool GET(const char *url, void (*callback)(String const &str,float &value), float &value) {
  HTTPClient main_client;
  main_client.begin(url);
  if (main_client.GET() == HTTP_CODE_OK) {
    Serial.println("GET REQUEST RESPONSE BEGIN");
    if (callback != 0) {
      callback(main_client.getString(),value);
    }
    delay(200);
    return true;
  }
  Serial.println("GET REQUEST RESPONSE BEGIN");
  
  return false;
}

bool POST(const char *url, void (*callback)(String const &str)) {
  HTTPClient main_client;
  main_client.begin(url);
  if (main_client.GET() == HTTP_CODE_OK) {
    Serial.println("POST REQUEST RESPONSE BEGIN");
    if (callback != 0) {
      callback(main_client.getString());
    }
    delay(200);
    return true;
  }
  Serial.println("POST REQUEST RESPONSE BEGIN");
  return false;
}

int get_request_int(String const &str) {
  int32_t tmp = str.toInt();
  return tmp;
}

float get_request_float(String const &str) {
  float tmp = str.toFloat();
  return tmp;
}
void get_request(String const &str, int32_t &value){
  value = get_request_int(str);
}
void get_request(String const &str, float &value){
  value = get_request_float(str);  
}

void get_request_raw_callback(String const &str) {
  Serial.println("GET REQUEST RESPONSE BEGIN");
  Serial.println("========================");
//  Serial.println(str.c_str());
//  Serial.println("========================");
  Serial.println("GET REQUEST RESPONSE END");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  serial_initialization();
  wifi_initialization();

  Serial.print("sizeof(ServerData): ");
  Serial.println((int)sizeof(ServerData));
  Serial.print("ESP READY!");
}
void print_get_server(String const &str,int32_t a, int32_t b){
  Serial.print(">>>> " + str);
  if (a == b){
    Serial.print(": On value: ");
    Serial.println(a);
  } else{
    Serial.print(": Off value: ");
    Serial.println(a);
  }
} 

void loop() {
  /* 1) Server polling data from server every 1500 ms
     2) Arduino always get local data
  */
  Serial.println("LOOP");
  Serial.print("sizeof(ServerData): ");
  Serial.println((int)sizeof(ServerData));
  Serial.print("sizeof(ProjectData): ");
  Serial.println((int)sizeof(ProjectData));
  uint32_t cur_time = millis();
  if (cur_time - last_sent_time > 1000) {
    //int variable

    GET("http://ecourse.cpe.ku.ac.th/exceed/api/ipalm-switchStatus/view/", get_request,server_data.switchStatus);
    delay(200);
    GET("http://ecourse.cpe.ku.ac.th/exceed/api/ipalm-motorStatus/view/", get_request,server_data.motorStatus);
    delay(200);
    GET("http://ecourse.cpe.ku.ac.th/exceed/api/ipalm-goRoom/view/", get_request,server_data.goRoom);
    Serial.println("<---- From Server ---->");
    
//    print_get_server("switchStatus", server_data.switchStatus, 0);
    print_get_server("motorStatus", server_data.motorStatus, 0);
    
    last_sent_time = cur_time;
  }

  while (se_read.available()) {
    char ch = se_read.read();
    if (cur_buffer_length == -1) {
      cur_data_header = ch;
      switch (cur_data_header) {
        case UPDATE_PROJECT_DATA:
          expected_data_size = sizeof(ProjectData);
          cur_buffer_length = 0;
          break;
        case GET_SERVER_DATA:
          expected_data_size = sizeof(ServerData);
          cur_buffer_length = 0;
          break;
      }
    } else if (cur_buffer_length < expected_data_size) {
      buffer[cur_buffer_length++] = ch;
      if (cur_buffer_length == expected_data_size) {
        switch (cur_data_header) {
          case UPDATE_PROJECT_DATA: {
              ProjectData *project_data = (ProjectData*)buffer;
              Serial.println("<---- From arduino ---- >");
//              int32_t sw;
//              int32_t led_Y; // air
//              int32_t led_B; //lightOut
//              int32_t led_W; //lightIn
//              float humid;
//              int32_t light; //ldr
              print_get_server("iPalm-switchStatus", project_data->sw, 0);
              POST(set_builder("iPalm-switchStatus", project_data->sw).c_str(), update_data_to_server_callback);
              print_get_server("iPalm-motorStatus", project_data->motor, 0);
              POST(set_builder("iPalm-motorStatus", project_data->motor).c_str(), update_data_to_server_callback);
//             
//              Serial.print("iPalm-humid: ");
//              Serial.println(project_data->humid);
//              POST(set_builder("iPalm-humid", project_data->humid).c_str(), update_data_to_server_callback);             

            }
            break;
          case GET_SERVER_DATA:
            Serial.println("<---- Send data to arduino ---->");
//            print_get_server("switchStatus", server_data.switchStatus, 0);
            print_get_server("motorStatus", server_data.motorStatus, 1);
            print_get_server("goRoom", server_data.goRoom, 1);
            send_to_arduino(GET_SERVER_DATA_RESULT, &server_data, sizeof(ServerData));
            break;
        }
        cur_buffer_length = -1;
      }
    }
  }
  delay(500);
}
