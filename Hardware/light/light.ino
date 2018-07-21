#define ls A0
#define ldr A1
//#define LED 5

int ldrValue = 0;
int lsValue = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(ldr, INPUT);
  pinMode(ls, INPUT);
//  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  pinMode(trigger_pin, OUTPUT);
  pinMode(in_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ldrValue = analogRead(ldr);
  lsValue = analogRead(ls);
  Serial.print("ldrValue: ");
  Serial.println(ldrValue);
  Serial.print("lightsen: ");
  Serial.println(lsValue);
  delay(1000);
//  if(ldrValue <= 50) {
//    digitalWrite(LED, HIGH);
//  }
//  else {
//    digitalWrite(LED, LOW);
//  }
}
