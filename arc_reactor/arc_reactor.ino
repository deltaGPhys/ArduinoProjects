//Basic demo sketch showing a few basic 'Arc Reactor Appropriate' animations
//Chris Barr, 2015

#include <Adafruit_NeoPixel.h>

#define PIN 11
#define NUM_PIXELS 25

int mode = 2; //change this number to see different effects
int num_modes = 3;
boolean automatically_change_mode = false; //disable this to lock to the one effect
unsigned long last_changed_mode_time = 0;
unsigned long effect_duration = 10000; //when changing automatically, each effect lasts for minimum 10s

int brightness = 60;
int mean_brightness = 260;
int flicker_range = 75;
int spin_rate = 50;
int fade_rate = 5;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t c_blue = strip.Color(10,10,255);

void setup() {
 strip.begin();
 strip.show(); // Initialize all pixels to 'off'
}

void loop() {
 
 if(mode==1)
 {
 //sets all NeoPixels to be blue
 setAll(c_blue);
 strip.show();
 }
 else if(mode==2)
 {
 //sets all NeoPixels to be blue with flicker
 brightness += random(flicker_range)-round(flicker_range/2);
 brightness = constrain(brightness, min(mean_brightness + flicker_range*5, 255), max(mean_brightness - flicker_range*5,0));
 setAll(strip.Color(10, 10, brightness));
 strip.show();
 }
 else if(mode==3)
 {
 //swirls around outer ring
 for(int i = 0; i < 24; i ++)
 {
 //fade change
 for(int j = 0; j < 50; j++)
 {
 //set previous pixel back to being dim
 if(i>0)
 strip.setPixelColor(i-1,strip.Color(10,10,200-j));
 else
 strip.setPixelColor(24,strip.Color(10,10,200-j));
 //set current pixel to be brighter
 strip.setPixelColor(i,strip.Color(10,10,150+j));
 
 //set center pixel to constant colour
 strip.setPixelColor(25,c_blue);
 
 //update changes
 strip.show();
 
 //wait before repeating
 delay(fade_rate);
 }
 delay(spin_rate);
 }
 
 }
 
 if(automatically_change_mode)
 {
 if(millis()-last_changed_mode_time > effect_duration)
 {
 last_changed_mode_time = millis();
 mode++;
 if(mode>num_modes)
 mode = 1;
 }
 }
 
}

void setAll(uint32_t color)
{
 for(int i = 0; i < NUM_PIXELS; i++)
 {
 strip.setPixelColor(i,color);
 }
}
