int LED[3] = {9,10,11};
int signalPins[3] = {6,7,8};
int numLED = 3;
boolean signals[3];

int dt = 10;
int len = 500;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  for (int i=0; i<numLED; i++){
    pinMode(LED[i],OUTPUT);
    pinMode(signalPins[i],INPUT);
    digitalWrite(signalPins[i],LOW);
    //digitalWrite(signals[i],LOW);
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
    signals[i] = digitalRead(signalPins[i]);
    if (signals[i] == HIGH) {
      digitalWrite(LED[i],HIGH);
    }
  }
  delay(len);
  for (int i=0; i<numLED; i++){
    digitalWrite(LED[i],LOW);
  }
  delay(dt);
}
