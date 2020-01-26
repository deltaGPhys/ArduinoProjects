
//Arduino 1.0+ Only
//Arduino 1.0+ Only

//////////////////////////////////////////////////////////////////

//Released under the MIT License - Please reuse change and share
//Simple code for the TMP102, simply prints ambient temperature via serial
//////////////////////////////////////////////////////////////////

#include <Wire.h>



int sensor = 11;
int ir = 10;
int led = 13;
int timer = 0;
int output = 12;


void setup(){
  Serial.begin(9600);
  Wire.begin();
  pinMode(sensor,INPUT);
  pinMode(led,OUTPUT);
  pinMode(ir,OUTPUT);
  pinMode(output,OUTPUT);
  
}

void loop(){

  timer = timer + 100;
  if (timer > 8000) {
    digitalWrite(ir,HIGH);
    
  }

  if (digitalRead (sensor)==HIGH) {
    digitalWrite(output,HIGH);
    digitalWrite(led,HIGH);
    
  } //else {
   // digitalWrite(coldoutput,LOW);
   // digitalWrite(led,LOW);
  //}

  
  
  delay(100); //just here to slow down the output. You can remove this
}


