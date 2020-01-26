/*
  
 */

int piNums[] = {
  1, 4, 1, 5, 9, 2, 6, 5, 3, 5, 9
  };
int piCount = 11;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins 2-9 (don't use 0 or 1!) as output.
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinTest();
  testNums();
}

void pinTest() {
  for (int i=2; i <= 9; i++){
    digitalWrite(i, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(250);              // wait for a second
    digitalWrite(i, LOW);    // turn the LED off by making the voltage LOW
  }    
}

void testNums(){
  for (int thisNum = 0 ; thisNum <= 9; thisNum++) {
    num(thisNum,500);
    delay(200);
  }
  decimalPt(500);
  delay(200);
}

void decimalPt(int time){
  digitalWrite(9, HIGH);
  delay(time);
  digitalWrite(9, LOW);
}

void num(int number, int time){
  if (number == 1) 
  { 
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    delay(time);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    
  }  
  else if (number == 2) 
  { 
    
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    delay(time);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    
  }  
  else if (number == 3) 
  { 
    
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(2, HIGH);
    delay(time);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
    digitalWrite(4, LOW);
    digitalWrite(8, LOW);
    digitalWrite(2, LOW);
    
  }  
  else if (number == 4) 
  { 
    
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    
    delay(time);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    digitalWrite(8, LOW);
    
    
  } 
 else if (number == 5) 
  { 
    
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(2, HIGH);
    delay(time);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(4, LOW);
    digitalWrite(8, LOW);
    digitalWrite(2, LOW);
    
  }   
  else if (number == 6) 
  { 
    
    digitalWrite(8, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    delay(time);
    digitalWrite(8, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    
  }  
  else if (number == 7) 
  { 
    
    digitalWrite(8, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    
    delay(time);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
   
    
  }  
  else if (number == 8) 
  { 
    
    digitalWrite(8, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    delay(time);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(4, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    
  } 
  else if (number == 9) 
  { 
    
    digitalWrite(8, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(2, HIGH);
    delay(time);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(4, LOW);
    digitalWrite(2, LOW);
    
  } 
  else if (number == 0) 
  { 
    
    digitalWrite(8, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(2, HIGH);
    delay(time);
    digitalWrite(8, LOW);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(3, LOW);
    digitalWrite(2, LOW);
    
  } 
  
}
// the loop function runs over and over again forever
void loop() {
  num(3,1000);
  decimalPt(1000);
 
  for (int thisNum = 0; thisNum < piCount; thisNum++) {
    num(piNums[thisNum], 1000);
  }
  delay(2000);
}
