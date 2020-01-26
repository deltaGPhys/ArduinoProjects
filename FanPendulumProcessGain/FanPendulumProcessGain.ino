#include "MotorDriver.h"

MotorDriver motor;


// Perform a manual change in fan output, measure process value (angle)
// response and time for that to happen, to determine process gain
// and process time constant
// Motor control, height, current error, set point, time variables
float motorSpeed = 0;
float t = 0;

float position = 0;

// Red - 5V
// Black - GND
const int encoder_a = 2; // Green - pin 2 - Digital
const int encoder_b = 3; // White - pin 3 - Digital
long encoder = 0;

void setup() {
  // Initialize serial communication:
  Serial.begin(9600);

  // wait for system to stop before starting, so that zero point is accurate
  while (Serial.available() == 0) { }

  // Initialize motor
  motor.begin();
  motor.speed(0, 54);

  // Rotary encoder pins
  pinMode(encoder_a, INPUT_PULLUP);
  pinMode(encoder_b, INPUT_PULLUP);

  attachInterrupt(0, encoderPinChangeA, CHANGE);
  attachInterrupt(1, encoderPinChangeB, CHANGE);     
}



void encoderPinChangeA() {
encoder += digitalRead(encoder_a) == digitalRead(encoder_b) ? -1 : 1;
position = float(encoder)*(360.0/2400.0); // convert position to angle in degrees (600 steps per turn, 2400 in quadrature)
}

void encoderPinChangeB() {
encoder += digitalRead(encoder_a) != digitalRead(encoder_b) ? -1 : 1;
position = float(encoder)*(360.0/2400.0); // convert position to angle in degrees (600 steps per turn, 2400 in quadrature)
}


void loop() {
  // Track the length of each loop, which varies with travel time
  // of the sound (which is dependent on the location of the ball)
  unsigned long StartTime = micros();
  
  if (t>2){
    motorSpeed = 30;
  }
  
  delay(100);
   
  unsigned long CurrentTime = micros();
  float ElapsedTime = float(CurrentTime - StartTime);

  motor.speed(0,motorSpeed);
  
  // Time update
  t = t + float(ElapsedTime/1000000);
  
  // Output to be copied and pasted into a CSV file that we'll make the graph from
  Serial.print(t);
  Serial.print(",");
 Serial.print(position);
  Serial.print(",");
  Serial.print("0");
  Serial.print(",");
  Serial.println(motorSpeed);
}


