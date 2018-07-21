#define alarmPin = 0;
#define alarmValue = 0;
void setup () 
{
      Serial.begin (9600);
      delay (2000);  
}
void loop ()
{
alarmValue = analogRead(alarmPin);
      Serial.println (alarmValue);
delay (500);
}
