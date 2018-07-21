void setup() {
  // put your setup code here, to run once:
  
}
int last_time = 0;
void loop() {
  // put your main code here, to run repeatedly:
  int cur_time = millis();
  if(cur_time - last_time > 2000) {

  last_time = cur_time;
  }
  
}
