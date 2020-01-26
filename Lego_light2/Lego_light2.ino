/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
Servo myservo2;
                // twelve servo objects can be created on most boards
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  myservo.attach(11);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(10);
} 
 
void loop() 
{ 
 myservo2.writeMicroseconds(1700);
 delay(2000);
 myservo2.writeMicroseconds(1519);
 delay(2000);
 myservo2.writeMicroseconds(1300);
 delay(2000);
} 

