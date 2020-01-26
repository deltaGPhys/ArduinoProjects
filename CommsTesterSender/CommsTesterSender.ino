int LED[3] = {9,10,11};
int signals[3] = {6,7,8};
int numLED = 3;

int dt = 10;
int between = 900;
int len = 500;
int pause = 5000;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for (int i=0; i<numLED; i++){
    pinMode(LED[i],OUTPUT);
    pinMode(signals[i],OUTPUT);
    digitalWrite(signals[i],LOW);
  }

  for (int j=0; j<4; j++){
    for (int i=0; i<numLED; i++){
      digitalWrite(LED[i],HIGH);
    }
    delay(150);
    for (int i=0; i<numLED; i++){
      digitalWrite(LED[i],LOW);
    }
    delay(150);
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for (int i=0; i<numLED; i++){
    Serial.println(i);
    for (int m=0; m<256; m++){
      analogWrite(LED[i],m);
      delay(10);
    }
    digitalWrite(LED[i],LOW);
    delay(100);

    digitalWrite(LED[i],HIGH);
    digitalWrite(signals[i],HIGH);
    delay(len);
    digitalWrite(LED[i],LOW);
    digitalWrite(signals[i],LOW);
    
    delay(between);
  } 
  delay(pause);
//  digitalWrite(LED1,HIGH);
//  delay(dt);
//  digitalWrite(LED1,LOW);
//  delay(between);
//
//  digitalWrite(LED2,HIGH);
//  delay(dt);
//  digitalWrite(LED2,LOW);
//  delay(between);
//
//  digitalWrite(LED3,HIGH);
//  delay(dt);
//  digitalWrite(LED3,LOW);
//  delay(between);
//  
//  delay(pause);
}
