
int xInput = A0;
int yInput = A1;
int x = 0;
int y = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(xInput,INPUT);
pinMode(yInput,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:


x = analogRead(xInput);
y = analogRead(yInput);

Serial.print("x: ");
Serial.print(x);
Serial.print(", y: ");
Serial.println(y);

delay(20);

}
