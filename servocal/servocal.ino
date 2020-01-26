// Servo zeroing assistant

#include <Servo.h>                           // Include servo library
 
Servo servoRight;                            // Declare right servo
Servo servoLeft;

int servospeed = 1490;

void setup(){
      // Servo setup
    //servoRight.attach(10);                     // Attach right signal to pin 10
    //servoRight.writeMicroseconds(servospeed);

    servoLeft.attach(11);                     // Attach left signal to pin 11
    servoLeft.writeMicroseconds(servospeed);

    Serial.begin(115200);
}


void loop(){
     // wait for ready
    Serial.print("press to increment speed; current servo speed: ");
    Serial.println(servospeed);
    
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());  // wait for data
    servospeed = servospeed + 1;
    while (Serial.available() && Serial.read()); // empty buffer again
    Serial.println(servospeed);
    servoLeft.writeMicroseconds(servospeed);        // 1.5 ms: stay still signal
    
  
}

