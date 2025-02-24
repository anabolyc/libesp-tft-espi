/*
  Sketch to show scrolling of the graphics in 4 bit sprites.
  Scrolling in this way moves the pixels in a defined rectangle
  within the Sprite. By default the whole sprite is scrolled.
  The gap left by scrolling is filled with a defined colour.

  Example for library:
  https://github.com/Bodmer/TFT_eSPI

  A Sprite is notionally an invisible graphics screen that is
  kept in the processors RAM. Graphics can be drawn into the
  Sprite just as it can be drawn directly to the screen. Once
  the Sprite is completed it can be plotted onto the screen in
  any position. If there is sufficient RAM then the Sprite can
  be the same size as the screen and used as a frame buffer.

  A 4 bit Sprite occupies (width * height)/2 bytes in RAM.
*/

#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite graph1 = TFT_eSprite(&tft); // Sprite object graph1

TFT_eSprite stext1 = TFT_eSprite(&tft); // Sprite object stext1

TFT_eSprite stext2 = TFT_eSprite(&tft); // Sprite object stext2

int graphVal = 1;
int delta = 1;
int grid = 0;
int tcount = 0;

// Palette colour table
uint16_t palette[16];

//==========================================================================================
void setup()
{
  Serial.begin(250000);
  tft.init();
  tft.fillScreen(TFT_BLACK);

  // Populate the palette table, table must have 16 entries
  palette[0] = TFT_BLACK;
  palette[1] = TFT_ORANGE;
  palette[2] = TFT_DARKGREEN;
  palette[3] = TFT_DARKCYAN;
  palette[4] = TFT_MAROON;
  palette[5] = TFT_PURPLE;
  palette[6] = TFT_OLIVE;
  palette[7] = TFT_DARKGREY;
  palette[8] = TFT_ORANGE;
  palette[9] = TFT_BLUE;
  palette[10] = TFT_GREEN;
  palette[11] = TFT_CYAN;
  palette[12] = TFT_RED;
  palette[13] = TFT_NAVY;
  palette[14] = TFT_YELLOW;
  palette[15] = TFT_WHITE;

  // Create a sprite for the graph
  graph1.setColorDepth(4);
  graph1.createSprite(128, 61);
  graph1.createPalette(palette);
  graph1.fillSprite(9); // Note: Sprite is filled with palette[0] colour when created

  // The scroll area is set to the full sprite size upon creation of the sprite
  // but we can change that by defining a smaller area using "setScrollRect()"if needed
  // parameters are x,y,w,h,color as in drawRect(), the color fills the gap left by scrolling

  //graph1.setScrollRect(64, 0, 64, 61, TFT_DARKGREY);  // Try this line to change the graph scroll area

  // Create a sprite for the scrolling numbers
  stext1.setColorDepth(4);
  stext1.createSprite(32, 64);
  stext1.createPalette(palette);
  stext1.fillSprite(9);                  // Fill sprite with palette colour 9 (blue in this example)
  stext1.setScrollRect(0, 0, 32, 64, 9); // here we set scroll gap fill color to blue
  stext1.setTextColor(15);               // Palette colour 15 (white) text, no background
  stext1.setTextDatum(BR_DATUM);         // Bottom right coordinate datum

  // Create a sprite for Hello World
  stext2.setColorDepth(4);
  stext2.createSprite(80, 16);
  stext2.createPalette(palette);
  stext2.fillSprite(7);
  stext2.setScrollRect(0, 0, 40, 16, 7); // Scroll the "Hello" in the first 40 pixels
  stext2.setTextColor(15);               // White text, no background
}

//==========================================================================================
void loop()
{
  // Draw point in graph1 sprite at far right edge (this will scroll left later)
  graph1.drawFastVLine(127, 60 - graphVal, 2, 14); // draw 2 pixel point on graph

  // Draw number in stext1 sprite at 31,63 (bottom right datum set)
  stext1.drawNumber(graphVal, 31, 63, 2); // plot value in font 2

  // Push the sprites onto the TFT at specified coordinates
  graph1.pushSprite(0, 0);
  stext1.pushSprite(0, 64);
  stext2.pushSprite(40, 70);

  // Change the value to plot
  graphVal += delta;

  // If the value reaches a limit, then change delta of value
  if (graphVal >= 60)
    delta = -1; // ramp down value
  else if (graphVal <= 1)
    delta = +1; // ramp up value

  delay(50); // wait so things do not scroll too fast

  // Now scroll the sprites scroll(dt, dy) where:
  // dx is pixels to scroll, left = negative value, right = positive value
  // dy is pixels to scroll, up = negative value, down = positive value
  graph1.scroll(-1, 0);  // scroll graph 1 pixel left, 0 up/down
  stext1.scroll(0, -16); // scroll stext 0 pixels left/right, 16 up
  stext2.scroll(1);      // scroll stext 1 pixel right, up/down default is 0

  // Draw the grid on far right edge of sprite as graph has now moved 1 pixel left
  grid++;
  if (grid >= 10)
  { // Draw a vertical line if we have scrolled 10 times (10 pixels)
    grid = 0;
    graph1.drawFastVLine(127, 0, 61, 13); // draw line on graph
  }
  else
  { // Otherwise draw points spaced 10 pixels for the horizontal grid lines
    for (int p = 0; p <= 60; p += 10)
      graph1.drawPixel(127, p, 13);
  }

  tcount--;
  if (tcount <= 0)
  { // If we have scrolled 40 pixels then redraw text
    tcount = 40;
    stext2.drawString("Hello World", 6, 0, 2); // draw at 6,0 in sprite, font 2
  }

} // Loop back and do it all again
//==========================================================================================
