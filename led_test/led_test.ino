int led = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led,true);
  delay(200);
  digitalWrite(led,false);
  delay(200);
  
}
