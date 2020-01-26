#include "MotorDriver.h"

MotorDriver motor;

const int pingPin = 7;
// Store the integral and derivative values, store the last error value to
// help calculate the derivative
float integral = 0;
float derivative = 0;
float lasterror = 0;
// PID constants
float Kp = 0.044;
float Ki = 0.011;
float Kd = 0.11792;
// Motor control, height, current error, set point, time variables
float motorSpeed = 0;
float height = 0;
float error = 0;
float setpoint = 50;
float t = 0;

void setup() {
  // Initialize serial communication:
  Serial.begin(9600);
  // Initialize motor
  motor.begin();
  motor.speed(0, 54);     
}

float measure() {
  // This function largely taken from the MotorDriver library examples
  // I have changed the time and distance variables to float, to increase
  // accuracy
  
  // Establish variables for duration of the ping,
  // and the distance result in centimeters:
  long duration;
  float cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  return cm;
}

float microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return float(microseconds) / 29.4117647 / 2.0;
}

void loop() {
  // Track the length of each loop, which varies with travel time
  // of the sound (which is dependent on the location of the ball)
  unsigned long StartTime = micros();

  // Measure distance, calculate current height
  height = 80.14 - measure();
  
  unsigned long CurrentTime = micros();
  float ElapsedTime = float(CurrentTime - StartTime);
  
  delay(100);
  // Calculate current error, integral, derivative, motor speed
  error = setpoint - height;
  integral = (ElapsedTime + 100000)*error/1000000 + integral;
  derivative = (error-lasterror)/(ElapsedTime);
  // Motor speed can't be negative!
  motorSpeed = max(Kp * error + Ki * integral + Kd*derivative, 0);
  motor.speed(0,motorSpeed);
  
  // This error is the next loop's previous error
  lasterror = error;
  // Time update (there's a little unaccounted-for time, but very little)
  t = t + float((ElapsedTime + 100000)/1000000);
  
  // Output to be copied and pasted into a CSV file that we'll make the graph from
  //Serial.print("Time: ");
  //Serial.print(ElapsedTime);
  Serial.print(t);
  Serial.print(",");
  //Serial.print(" us // ");
  Serial.print(height);
  Serial.print(",");
  //Serial.print(" cm // set: ");
  Serial.print(setpoint);
  Serial.print(",");
  //Serial.print(" cm // ");
  Serial.println(motorSpeed);
}


