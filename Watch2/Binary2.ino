// Binary watch display.  Hours are shown as a 4-bit value across the top,
// using 2x2 squares.  Minutes and seconds and broken out into high and
// low BCD digits, displayed as 3- and 4-bit values, 1 pixel per bit.

#define BACKGROUND 0
#define BIT_CLEAR 32
#define BIT_SET  255

void mode_binary2(uint8_t action) {
  DateTime now;
  uint8_t  h, x, bit, b_set, b_clear, depth;
  uint16_t t;

  if(action != ACTION_NONE) {
    // If we just arrived here (whether through mode change
    // or wake from sleep), initialize the matrix driver:
    if(action >= ACTION_HOLD_LEFT) {
      uint8_t plex = LED_PLEX_2;
      depth = 4;
      // Reduce depth/plex if battery voltage is low
      if(watch.getmV() < BATT_LOW_MV) {
        depth = 2;
        plex  = LED_PLEX_1;
      }
      // Reconfigure display if needed
      if((watch.getDepth() != depth) || (watch.getPlex() != plex))
        fps = watch.setDisplayMode(depth, plex, true);
    }
    // Reset sleep timeout on ANY button action
    watch.setTimeout(fps * 8);
  }

  now = RTC.now();
  h   = now.hour();
  if     (h  > 12) h -= 12;
  else if(h ==  0) h  = 12;
  loadDigits(now.minute(), DIGIT_MIN);
  loadDigits(now.second(), DIGIT_SEC);

  // Calc set/clear colors based on current fadeout value
  depth = watch.getDepth();
  if((t = watch.getTimeout()) < sizeof(fade)) {
    uint16_t b1 = (uint8_t)pgm_read_byte(&fade[t]) + 1;
    b_set       = (BIT_SET   * b1) >> (16 - depth);
    b_clear     = (BIT_CLEAR * b1) >> (16 - depth);
  } else {
    b_set       =  BIT_SET   >> (8 - depth);
    b_clear     =  BIT_CLEAR >> (8 - depth);
  }

  watch.fillScreen(BACKGROUND);
  // Draw hour as 2x2 blocks
  for(x = 0, bit = 0x08; bit; bit >>= 1, x += 2) {
    watch.fillRect(x, 1, 2, 2, (h & bit) ? b_set : b_clear);
  }
  // Draw minutes, seconds as 6-bit values
  for(x = 0, bit = 0x64; bit; bit >>= 1, x++) {
    watch.drawPixel(x, 4, (digit[DIGIT_MIN] & bit) ? b_set : b_clear);
    watch.drawPixel(x, 6, (digit[DIGIT_SEC] & bit) ? b_set : b_clear);
  }
}

