#include<Servo.h>
Servo servo1;
Servo servo2;
int xjoy=A0;
int yjoy=A1;
int xval;
int yval;
float xmot;
float ymot;
int outputs[5];
int count=0;
int totalx;
int avgy;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
servo1.attach(9);
servo2.attach(10);
pinMode(xjoy,INPUT);
pinMode(yjoy,INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
totalx=0;
xval=analogRead(xjoy);
Serial.println(analogRead(xjoy));


yval=analogRead(yjoy);
xmot=map(xval,0,1023,0,160);
ymot=map(yval,0,1023,0,160);
outputs[count]=xmot;
count=(count+1)%5;

for (int i=0; i <= 4; i++){
  totalx=totalx+outputs[i];

  
}

float value=float(totalx)/5.0;
servo2.write(value);


delay(5);

}
