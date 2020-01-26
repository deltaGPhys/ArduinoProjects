
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
int IR = 3; //on/off 
int IR2 = 4; //adjustments
int pinSet = 0;
int pins[4] = {0,11,21,32};
unsigned long timestart = 0;
unsigned long timestop = 0;
boolean triggered = false;
int colors[9][3]= {
  {255,255,255},//White
  {255,0,0}, //Red
  {255,127,0}, //Orange
  {255,255,0},//Yellow
  {0,255,0},//Green
  {0,0,255},//Blue
  {0,255,255},//Cyan
  {127,0,255},//Purple
  {255,255,255},
};
int colorChosen = 0;
float Intensity = 1;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, PIN, NEO_RGB + NEO_KHZ400);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
 // #if defined (__AVR_ATtiny85__)
 //   if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
 // #endif
  Serial.begin(9600);
  // End of trinket special code
  pinMode(IR,INPUT);
  digitalWrite(IR, HIGH); // turn on the pullup
  pinMode(IR2,INPUT);
  digitalWrite(IR2, HIGH); // turn on the pullup
  pinMode(13,OUTPUT);
  //attachInterrupt(digitalPinToInterrupt(IR), IRSwitch, HIGH);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("Lampy operational, boss");
  digitalWrite(13,HIGH);
  delay(200);
  digitalWrite(13,LOW);
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
//  while (true) {
//    Serial.print("1:");
//    Serial.print(digitalRead(IR));
//    Serial.print(" 2:");
//    Serial.println(digitalRead(IR2));
//  }
  if (digitalRead(IR) == HIGH){
      digitalWrite(13,HIGH);
    delay(50);
    digitalWrite(13,LOW);
    delay(500);
    } 

    if (digitalRead(IR2) == HIGH){
      digitalWrite(13,HIGH);
    delay(400);
    digitalWrite(13,LOW);
 
    delay(500);
    } 
}
