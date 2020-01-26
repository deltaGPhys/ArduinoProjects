
// Potentiometer test client

int KpPin = A0;
int KiPin = A1;
int KdPin = A2;

float KpMax = 1; // maximum values for the coefficients
float KiMax = .3;
float KdMax = .3;

void setup() {
  // put your setup code here, to run once:
  pinMode(KpPin, INPUT);
  pinMode(KiPin, INPUT);
  pinMode(KdPin, INPUT);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  Serial.print("Kp: ");
  Serial.print(map(analogRead(KpPin),0,1024,0,1000.0*KpMax)*1.163/1000.0);
  Serial.print(", Ki: ");
  Serial.print(map(analogRead(KiPin),0,1024,0,1000.0*KiMax)*1.163/1000.0);
  Serial.print(", Kd: ");
  Serial.println(map(analogRead(KdPin),0,1024,0,1000.0*KdMax)*1.163/1000.0);

}
