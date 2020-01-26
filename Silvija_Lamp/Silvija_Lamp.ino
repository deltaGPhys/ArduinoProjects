#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 65

int sidelength = 13;
int numsides = 5;
int side = 0;
int lednum = 0;

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void(* resetFunc) (void) = 0; //declare reset function at address 0

void loop() { 
  // Turn the LED set on, then pause
  side = 0;
  while (side<numsides){
    lednum = 0;
    while (lednum<sidelength){
      leds[lednum+side*sidelength] = CRGB::White;
      lednum++;
    }
    FastLED.show();
    delay(500);
    
    lednum = 0;
    while (lednum<sidelength){
      leds[lednum+side*sidelength] = CRGB::Black;
      lednum++;
    }
    FastLED.show();
    side++;
  }

  resetFunc(); //call reset 
}
