// Demo using arcFill to draw ellipses and a segmented elipse
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

#define LOOP_DELAY 10 // Loop delay to slow things down

byte inc = 0;
unsigned int col = 0;

byte red = 31;  // Red is the top 5 bits of a 16 bit colour value
byte green = 0; // Green is the middle 6 bits
byte blue = 0;  // Blue is the bottom 5 bits
byte state = 0;

// #########################################################################
// Return the 16 bit colour with brightness 0-100%
// #########################################################################
unsigned int brightness(unsigned int colour, int brightness)
{
  byte red = colour >> 11;
  byte green = (colour & 0x7E0) >> 5;
  byte blue = colour & 0x1F;

  blue = (blue * brightness) / 100;
  green = (green * brightness) / 100;
  red = (red * brightness) / 100;

  return (red << 11) + (green << 5) + blue;
}

// #########################################################################
// Return a 16 bit rainbow colour
// #########################################################################
unsigned int rainbow(byte value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to 127 = red

  switch (state)
  {
  case 0:
    green++;
    if (green == 64)
    {
      green = 63;
      state = 1;
    }
    break;
  case 1:
    red--;
    if (red == 255)
    {
      red = 0;
      state = 2;
    }
    break;
  case 2:
    blue++;
    if (blue == 32)
    {
      blue = 31;
      state = 3;
    }
    break;
  case 3:
    green--;
    if (green == 255)
    {
      green = 0;
      state = 4;
    }
    break;
  case 4:
    red++;
    if (red == 32)
    {
      red = 31;
      state = 5;
    }
    break;
  case 5:
    blue--;
    if (blue == 255)
    {
      blue = 0;
      state = 0;
    }
    break;
  }
  return red << 11 | green << 5 | blue;
}

void setup(void)
{
  tft.begin();
  tft.setRotation(TFT_ROTATION);
  tft.fillScreen(TFT_BLACK);
}

void loop()
{

  // Continuous elliptical arc drawing
  tft.fillArc(tft.width() >> 1, tft.height() >> 1, 
    inc * 6, 1, 
    tft.width() >> 1, tft.height() >> 1, 
    10, rainbow(col));

  // Continuous segmented (inc*2) elliptical arc drawing
  tft.fillArc(tft.width() >> 1, tft.height() >> 1, 
    ((inc * 2) % 60) * 6, 1, 
    tft.width() / 3, tft.height() / 3, 
    30, rainbow(col));

  // Circle drawing using arc with arc width = radius
  tft.fillArc(tft.width() >> 1, tft.height() >> 1, 
    inc * 6, 1, 
    tft.height() / 5, tft.height() / 5, 
    42, rainbow(col));

  inc++;
  col += 1;
  if (col > 191)
    col = 0;
  if (inc > 59)
    inc = 0;

  delay(LOOP_DELAY);
}
