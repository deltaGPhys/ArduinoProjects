
//Arduino 1.0+ Only
//Arduino 1.0+ Only

//////////////////////////////////////////////////////////////////

//Released under the MIT License - Please reuse change and share
//Simple code for the TMP102, simply prints ambient temperature via serial
//////////////////////////////////////////////////////////////////

#include <Wire.h>


int tmp102Address = 0x48; /* ADD0 tied to ground */
int coldoutput = 8;
int led = 13;
float crook;

void setup(){
  Serial.begin(9600);
  Wire.begin();
  pinMode(coldoutput,OUTPUT);
  pinMode(led,OUTPUT);
  crook = calibrate();
  Serial.print("Calibration temp: ");
  Serial.print(crook);
  Serial.print(" ");
  Serial.write(0xC2);
  Serial.write(0xB0);
  Serial.print("F");
  Serial.write(9);
  Serial.println(" ");
}

void loop(){

  
  float fahrenheit = getTemperature() - crook;
  Serial.print(fahrenheit);
  Serial.print(" ");
  Serial.write(0xC2);
  Serial.write(0xB0);
  Serial.print("F");
  Serial.write(9);
  Serial.println(" ");

  if (fahrenheit < -.8) {
    digitalWrite(coldoutput,HIGH);
    digitalWrite(led,HIGH);
  } //else {
   // digitalWrite(coldoutput,LOW);
   // digitalWrite(led,LOW);
  //}

  
  
  delay(100); //just here to slow down the output. You can remove this
}

float getTemperature(){
  Wire.requestFrom(tmp102Address,2); 

  byte MSB = Wire.read();
  byte LSB = Wire.read();

  //it's a 12bit int, using two's compliment for negative
  int TemperatureSum = ((MSB << 8) | LSB) >> 4; 

  float celsius = TemperatureSum*0.0625;
  return (1.8 * celsius) + 32;
}

float calibrate(){
  
  return getTemperature();
}


