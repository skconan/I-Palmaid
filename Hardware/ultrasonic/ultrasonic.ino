#define trigger_pin 7
#define in_pin 6
long duration, cm;

long microsecondsToCentimeters( long microseconds)
{
  return microseconds / 29 / 2;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigger_pin, OUTPUT);
  pinMode(in_pin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  duration = pulseIn(in_pin, HIGH);
  cm = microsecondsToCentimeters(duration);
  Serial.println(cm);
  delay(500);
}
