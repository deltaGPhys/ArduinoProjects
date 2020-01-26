/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 9 has an LED connected on most Arduino boards.
// give it a name:
int led1 = 5;
int led2 = 9;
int led3 = 13;
int led4 = 3;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  shutIt();
}



// the loop routine runs over and over again forever:
void loop() {
  blinker(led3,led2);
  blinker(led4,led2);
  blinker(led1,led2);
  
  blinker(led2,led1);
  blinker(led3,led1);
  blinker(led4,led1);
  
  blinker(led1,led4);
  blinker(led2,led4);
  blinker(led3,led4);
  
  blinker(led4,led3);
  blinker(led1,led3);
  blinker(led2,led3);
  
  shutIt();
  
  delay(2000);
  
}

void explorer(){

  twoLights(led1,led2); // 1B, 2R
  twoLights(led1,led3); // 4G, 2G
  twoLights(led1,led4); // 3R, 2B
  twoLights(led2,led3); // 4B, 1R
  twoLights(led2,led4); // 3G, 1G
  twoLights(led3,led4); // 3B, 4R
  shutIt();
  delay(3000);
  
  
}

void blinker(int firstLight,int secondLight) {
  shutIt();
  
  pinMode(firstLight, OUTPUT);
  pinMode(secondLight, OUTPUT); 
 
  digitalWrite(firstLight, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(secondLight, LOW);
  delay(1000);   
  
  
}

// swap a pair of pins
void twoLights(int firstLight,int secondLight) {
  shutIt();
  
  pinMode(firstLight, OUTPUT);
  pinMode(secondLight, OUTPUT); 
 
  digitalWrite(firstLight, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(secondLight, LOW);
  delay(1000);   
  // wait for a second
  digitalWrite(secondLight, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(firstLight, LOW);
  delay(1000);  
  
}

void shutIt() {
  pinMode(led1, INPUT);
  pinMode(led2, INPUT);
  pinMode(led3, INPUT);
  pinMode(led4, INPUT);
   
}
