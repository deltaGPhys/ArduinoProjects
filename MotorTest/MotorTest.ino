// ConstantSpeed.pde
// -*- mode: C++ -*-
//
// Shows how to run AccelStepper in the simplest,
// fixed speed mode with no accelerations
// Requires the AFMotor library (https://github.com/adafruit/Adafruit-Motor-Shield-library)
// And AccelStepper with AFMotor support (https://github.com/adafruit/AccelStepper)
// Public domain!

#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

int FL = 4;
int RL = 5;
int FR = 6;
int RR = 7;

void setup()
{  
   Serial.begin(9600);           // set up Serial library at 9600 bps
   pinMode(FL,OUTPUT);
   pinMode(RL,OUTPUT);
   pinMode(FR,OUTPUT);
   pinMode(RR,OUTPUT);
   
   
}

void loop()
{  
  
   motor1.setSpeed(255);
   motor2.setSpeed(255);
   motor3.setSpeed(255);
   motor4.setSpeed(255);

   digitalWrite(FL,HIGH);
   delay(1000);
   digitalWrite(FL,LOW);
   digitalWrite(FR,HIGH);
   delay(1000);
   digitalWrite(FR,LOW);
   digitalWrite(RL,HIGH);
   delay(1000);
   digitalWrite(RL,LOW);
   digitalWrite(RR,HIGH);
   delay(1000);
   digitalWrite(RR,LOW);
   delay(4000);
   
   
}
