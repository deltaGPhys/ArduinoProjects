#include "MotorDriver.h"

MotorDriver motor;


// Store the integral and derivative values, store the last error value to
// help calculate the derivative
float integral = 0;
float derivative = 0;
float lasterror = 0;
// PID constants
float Kp = .5;
float Ki = .28;
float Kd = 0;
// Motor control, height, current error, set point, time variables
float motorSpeed = 0;
float height = 0;
float error = 0;
float setpoint = 13;
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
  position = float(encoder) * (360.0 / 2400.0); // convert position to angle in degrees (600 steps per turn, 2400 in quadrature)
}

void encoderPinChangeB() {
  encoder += digitalRead(encoder_a) != digitalRead(encoder_b) ? -1 : 1;
  position = float(encoder) * (360.0 / 2400.0); // convert position to angle in degrees (600 steps per turn, 2400 in quadrature)
}


void loop() {
  // Track the length of each loop, which varies with travel time
  // of the sound (which is dependent on the location of the ball)
  unsigned long StartTime = micros();

  if (t > 45) {
    setpoint = 18;
  }
  if (t > 90) {
    setpoint = 8;
  }
  delay(100);

  // Calculate current error, integral, derivative, motor speed
  error = setpoint - position;

  unsigned long CurrentTime = micros();
  float ElapsedTime = float(CurrentTime - StartTime);
  integral = ElapsedTime * error / 1000000 + integral;

  derivative = (error - lasterror) / (ElapsedTime);

  // Motor speed can't be negative!
  motorSpeed = max(Kp * error + Ki * integral + Kd * derivative, 0);
  motorSpeed = min(motorSpeed, 100);
  motor.speed(0, motorSpeed);

  // This error is the next loop's previous error
  lasterror = error;
  // Time update
  t = t + float(ElapsedTime / 1000000);

  // Output to be copied and pasted into a CSV file that we'll make the graph from
  Serial.print(t);
  Serial.print(",");
  Serial.print(position);
  Serial.print(",");
  Serial.print(setpoint);
  Serial.print(",");
  Serial.println(motorSpeed);
}


