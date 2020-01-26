#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 8
CRGB leds[50];

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS); 
  leds[2] = CRGB::Blue; 
  FastLED.show(); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
