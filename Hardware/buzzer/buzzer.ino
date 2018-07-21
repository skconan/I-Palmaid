#define BUZZER 3
#define sw 2

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
  //int tones[] = { 264, , 1519, 1432, 1275, 1136, 1014, 956 };
  Serial.print(note);
  // play the tone corresponding to the note name
  if(note != 0){
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(tone[note]);
    digitalWrite(BUZZER, LOW);
    delayMicroseconds(tone[note]);
    /*tone(BUZZER, tone[note]);
    delay(400);
    noTone(BUZZER);
    delay(200);
  }*/}
  else {
    delay(800);
    
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
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUZZER ,OUTPUT);
  pinMode(sw, INPUT);
}

void loop() {
  if(digitalRead(sw) == 0){
    ringOn();
  }
  // put your main code here, to run repeatedly:
  /*analogWrite(buzzer, HIGH);
  delay(1000);
  analogWrite(buzzer, LOW);
  delay(1000);*/  
}
