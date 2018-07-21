#define SWITCH 5
#define LED 6

int inp;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(SWITCH, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  inp = digitalRead(SWITCH);
  if(inp == 0) {
    digitalWrite (LED, HIGH);
  }
  else {
    digitalWrite (LED, LOW);
  }
}
