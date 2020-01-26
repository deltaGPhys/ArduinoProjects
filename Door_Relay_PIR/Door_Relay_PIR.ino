
int relay = 3;
int outputLED = 6;
// defines pins numbers
const int trigPin = 4;
// defines variables
long duration;
long dist;
long distance;


void setup() {

  pinMode(outputLED, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(trigPin, INPUT);

  // Relay output pin
  pinMode(relay, OUTPUT);
}




void LEDstrobe() {
  long t = 0;
  while (t < 8000) {
    analogWrite(outputLED, 125 - 125 * cos(2.0 * 3.14 * t / 1000.0));
    //analogWrite(outputLED,324+t*1000/700);
    delay(5);
    t = t + 5;
  }
  analogWrite(outputLED, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, LOW);



  if (digitalRead(trigPin) == HIGH) {
    digitalWrite(relay, HIGH);
    digitalWrite(13, HIGH);
    LEDstrobe();
    digitalWrite(relay, LOW);
    digitalWrite(13, LOW);
  }


  delay(500);
}
