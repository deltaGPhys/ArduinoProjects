/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */

int sensorPin = A0;    // select the input pin for the potentiometer
int currentPin = A1;  // input pin for current sensor
int outPin = 9;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
float Vin=0;
float Vout = 2.2; // Vout
int Voutpin = 2.2*1023.0/5.0;
float currentValue = 0; // input from current sensor
//int currentPin = 0;

void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  pinMode(outPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  pinMode(currentPin, INPUT); 
  
}

void loop() {
  // read the value from the sensor:
  analogWrite(outPin,Voutpin);
  sensorValue = analogRead(sensorPin);
  Vin = sensorValue*5/1023
  currentPin = analogRead(currentPin);
  //currentValue = currentPin*
  
  // print voltage
  
  Serial.println("V out");Serial.println(Vout);
  
  Serial.println("V in");Serial.println(Vin);
  //Serial.println("i in");Serial.println(currentValue);
  delay(2000); 
  // turn the ledPin on
  //digitalWrite(ledPin, HIGH);  
  // stop the program for <sensorValue> milliseconds:
  //delay(sensorValue);          
  // turn the ledPin off:        
  //digitalWrite(ledPin, LOW);   
  // stop the program for for <sensorValue> milliseconds:
  //delay(sensorValue);                  
}
