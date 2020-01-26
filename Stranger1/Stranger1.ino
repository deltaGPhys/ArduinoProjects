#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 8
  
CRGB leds[NUM_LEDS];
  
void setup() {
  
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  leds[0] = CRGB::Red; 
  FastLED.show(); 
  delay(30); 
}
