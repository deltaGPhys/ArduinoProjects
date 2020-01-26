
int red = 9;
int green = 5;
int blue = 3;

int switchpin = 2;


void setup() {
  // put your setup code here, to run once:
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(switchpin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(switchpin)){
    
  
  digitalWrite(red,HIGH);
  delay(1000);
  digitalWrite(red,LOW);
  digitalWrite(green,HIGH);
  delay(1000);
  digitalWrite(green,LOW);
  digitalWrite(blue,HIGH);
  delay(1000);
  digitalWrite(blue,LOW);
  }
  
}
