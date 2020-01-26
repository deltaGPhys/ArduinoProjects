int LED1 = 3; //PWM
int LED2 = 5; //PWM
int LED3 = 6; //PWM
int LED4 = 9; //PWM
int LED5 = 10; //PWM
int LED6 = 11; //PWM
  
void setup() {
  // put your setup code here, to run once:
  
  pinMode(OUTPUT,LED1);
  pinMode(OUTPUT,LED2);
  pinMode(OUTPUT,LED3);
  pinMode(OUTPUT,LED4);
  pinMode(OUTPUT,LED5);
  pinMode(OUTPUT,LED6);
  
}

int between = 2000; //interval between blinks
int blinker = 5; //delay in fade loop

void blinky(int LED){
  int brightness = 1023;
  while(brightness > 600){
    analogWrite(LED,brightness);
    brightness--;
    delay(blinker);
  }
  while(brightness < 1023){
    analogWrite(LED,brightness);
    brightness++;
    delay(blinker);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  int timer = 0;
  analogWrite(LED1,1023);
  analogWrite(LED2,1023);
  analogWrite(LED3,1023);
  analogWrite(LED4,1023);
  analogWrite(LED5,1023);
  analogWrite(LED6,1023);
  
  while (timer<=7000){
    if (timer = 1000){
      blinky(LED1);
    }
    if (timer = 2500){
      blinky(LED2);
    }
    if (timer = 3000){
      blinky(LED3);
    }
    if (timer = 4000){
      blinky(LED4);
    }
    if (timer = 4500){
      blinky(LED5);
    }
    if (timer = 6000){
      blinky(LED6);
    }
    delay(1);
    timer = timer + 1;
  }

}
