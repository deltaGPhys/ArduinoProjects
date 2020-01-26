
//Arduino 1.0+ Only
//Arduino 1.0+ Only

//////////////////////////////////////////////////////////////////

//Released under the MIT License - Please reuse change and share
//Simple code for the TMP102, simply prints ambient temperature via serial
//////////////////////////////////////////////////////////////////

#include <Wire.h>

int coldoutput = 7;
int led = 13;
float calib;
int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the sensor divider
int timer = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  pinMode(led, OUTPUT);
  pinMode(coldoutput, OUTPUT);

  calib = analogRead(photocellPin);
  Serial.print("Calibration brightness: ");
  Serial.println(analogRead(photocellPin));
}

void loop() {

  photocellReading = analogRead(photocellPin) - calib;

  Serial.print("Analog reading = ");
  Serial.println(photocellReading);     // the raw analog reading

  if (photocellReading > 100 ) {
    timer = timer + 100;
  }
  
  if (photocellReading > 100 && timer > 9000) {
    digitalWrite(coldoutput, HIGH);
    digitalWrite(led, HIGH);
  } //else {
  // digitalWrite(coldoutput,LOW);
  // digitalWrite(led,LOW);
  //}



  delay(100); //just here to slow down the output. You can remove this
  
}



