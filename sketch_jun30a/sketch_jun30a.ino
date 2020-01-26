void setup() {
  pinMode(13, OUTPUT);
}
int change2 = 5;
int number2 = 50;
int number = 1000;
int change = number2;
void loop() {
  digitalWrite(13, HIGH);
  delay(number);
  digitalWrite(13, LOW);
  delay(number);
  number = number - change;
  if(number == 0) {
    change = change * -1;
    number2 = number2 - change2;
  }
  if(number == 1000) {
    change = change * -1;
    number = number2 - change2;
  }
  if(number2 = 0) {
    change2 = change2 * -1;
  }
  if(number2 = 50) {
    change2 = change2 * -1;
  }
}
