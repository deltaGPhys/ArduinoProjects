
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
int colors[11][3]= {
  {255,255,255},//White
  {255,0,0}, //Red
  {255,127,0}, //Orange
  {255,255,0},//Yellow
  {0,255,0},//Green
  {0,0,255},//Blue
  {0,255,255},//Cyan
  {127,0,255},//Purple
  {255,255,255},
  {255,255,255},
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
  
  //distance sensor if beam not broken - change intensity
  if (digitalRead(IR2) == LOW && digitalRead(IR) == HIGH) {// beam 2 only broken
    timestart = millis(); // set start of interval time
    
    if (digitalRead(IR) == HIGH){
      digitalWrite(13,HIGH);
    delay(50);
    digitalWrite(13,LOW);
    delay(50);
    digitalWrite(13,HIGH);
    delay(50);
    digitalWrite(13,LOW);
    delay(50);
    digitalWrite(13,HIGH);
    delay(200);
    digitalWrite(13,LOW);
    delay(200);
    } 
    
    while (digitalRead(IR2) == LOW) { //change the color as long as you hold
      delay(400);
      colorChosen++; // next color
      colorChosen = colorChosen % 11;
      Serial.print("New color: ");
      Serial.println(colorChosen);
      showDots(strip.Color(colors[colorChosen][0]*Intensity,colors[colorChosen][1]*Intensity,colors[colorChosen][2]*Intensity)); 
    }
  }
  
  if (digitalRead(IR) == LOW && digitalRead(IR2) == HIGH){// beam 1 broken only
    timestart = millis(); // set start of interval time
    timestop = millis();
    while (digitalRead(IR) == LOW) {
      timestop = millis();// update interval end time
      // check for distance sensor
      // change color by stepping through
      if (timestop-timestart >= 1500){// hold it long enough to change intensity
      
        while (digitalRead(IR) == LOW) { //change the intensity as long as you hold
          timestop = millis();
          delay(20);
          if (timestop-timestart < 10000) {
            Intensity += .02; // next level
            if (Intensity > 1) { Intensity = Intensity - 1;}
          } else {
            Intensity = 1;
          }
          Serial.print("New intensity: ");
          Serial.println(Intensity);
          showDots(strip.Color(colors[colorChosen][0]*Intensity,colors[colorChosen][1]*Intensity,colors[colorChosen][2]*Intensity)); 
        }
      }
    }
    Serial.print("Time: ");
    Serial.println(timestop-timestart);
    if (timestop-timestart > 50 && timestop-timestart < 1500) {// next display mode (short touch)
      pinSet = pinSet + 1;
      pinSet = pinSet % 4;
      Serial.print("New light set ");
      Serial.println(pinSet);

      if (pinSet != 0) {
        colorWipe(strip.Color(colors[colorChosen][0]*Intensity,colors[colorChosen][1]*Intensity,colors[colorChosen][2]*Intensity),50); // White 
      } else {
        reverseColorWipe(25);
        delay(500);
      }
    }
  }
  
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue

  //rainbow(20);
  //rainbowCycle(20);
  //theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pins[pinSet]; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Kill the dots one after the other with a color
void reverseColorWipe(uint8_t wait) {
  Serial.println("erase");
  for(int i=32; i>=0; i--) {
    strip.setPixelColor(i, strip.Color(0,0,0));
    strip.show();
    delay(wait);
  }
  Serial.println("done erasing");
}

// Show the dots, no delay
void showDots(uint32_t c) {
  for(uint16_t i=0; i<pins[pinSet]; i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void IRSwitch() {
  pinSet = pinSet + 1;
  pinSet = pinSet % 4;
  Serial.print("Triggered: new set ");
  Serial.println(pinSet);
 
}
