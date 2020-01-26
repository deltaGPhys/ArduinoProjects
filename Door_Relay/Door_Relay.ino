
int relay = 3;
int outputLED = 6;
// defines pins numbers
const int trigPin = 4;
const int echoPin = 5;
// defines variables
long duration;
long dist;
long distance;


void setup() {

  pinMode(outputLED,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // Relay output pin
  pinMode(relay, OUTPUT);
}

float measure() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  dist= duration*0.034/2;
  return dist;
}


void LEDstrobe(){
  long t = 0;
  while (t<7000){
    analogWrite(outputLED,125-125*cos(2.0*3.14*t/1000.0));
    //analogWrite(outputLED,324+t*1000/700);
    delay(5);
    t=t+5;
  }
  analogWrite(outputLED,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13,LOW);
  distance = measure();
  
  digitalWrite(13,LOW);
  if (distance<200){
    float summer = 0;
    float n = 0;
    while (n<5){
      summer = summer + measure();
      n = n + 1;  
      delay(5);
    }
    float avg = summer/n;
    if (avg < 200){
      digitalWrite(relay,HIGH);
      digitalWrite(13,HIGH);
      LEDstrobe();
      digitalWrite(relay,LOW);
      digitalWrite(13,LOW);
    }
  }
  
  delay(500);
}
