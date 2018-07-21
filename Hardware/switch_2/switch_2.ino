#define SWITCH 5
#define LED 6

int inp;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(SWITCH, INPUT);
  Serial.begin(9600);
}

int ledOn = 0;

void loop() {
  // put your main code here, to run repeatedly:
  inp = digitalRead(SWITCH);
  
  if(inp == 0) {
    
    while(inp == 0){
      inp = digitalRead(SWITCH);
    }
    
    if(ledOn == 0){
      digitalWrite (LED, HIGH);
      ledOn = 1;
      Serial.println(inp);
    }
    
    else{
      digitalWrite (LED, LOW);
      ledOn = 0;
      Serial.println(inp);
    }
  }
  
}
