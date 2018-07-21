#define alarmPin 16

void setup () 
{
      Serial.begin (9600);
      delay (2000);  
}
void loop ()
{
//alarmValue = analogRead(alarmPin);
      Serial.println (analogRead(alarmPin));
delay (500);
}
