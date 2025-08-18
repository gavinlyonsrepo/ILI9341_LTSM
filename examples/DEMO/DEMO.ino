/*! 
	@file   DEMO.ino
	@brief  Demo Unit Circle with Sine & Cos Wave for ili9341 display
	@author Gavin Lyons.
	@note   See USER OPTIONS 1-2 in SETUP function
	@test 
		-# 450 Demo Unit Circle with Sine & Cos Wave
*/

// libraries
#include "ILI9341_LTSM.hpp"
// Fonts needed
#include <fonts_LTSM/FontMega_LTSM.hpp>

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

ILI9341_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
  Serial.begin(38400);
  delay(1000);
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 5;
  int8_t RST_TFT = 4;
  int8_t CS_TFT = 15;
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
    myTFT.SetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_TFT = 13;
    int8_t SCLK_TFT = 12;
    myTFT.SetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  uint16_t TFT_WIDTH = 240;   // Screen width in pixels
  uint16_t TFT_HEIGHT = 320;  // Screen height in pixels
  myTFT.SetupScreenSize(TFT_WIDTH, TFT_HEIGHT);
  // ===
  myTFT.ILI9341Initialize();
  Serial.println("Start");
  delay(1000);
}

// MAIN loop
void loop(void) {
  myTFT.setRotation(myTFT.Degrees_90);
  demo();
  EndTests();
}

// Function Space
void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.PowerDown();  // Power down device
  Serial.println("End");
  while (1) {};
}

void demo(void) {
  Serial.println("Unit Circle & Sine & Cos Wave");

  int16_t CenterCircleX = 24;
  int16_t CenterCircleY = 64;
  int16_t Radius = 23;
  int16_t waveStartX = 48;            // Start position for sine wave
  int16_t waveBaseY = CenterCircleY;  // Align sine wave with center

  myTFT.fillScreen(myTFT.C_BLACK);
  char buffer[4];

  myTFT.setFont(FontMega);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.drawCircle(CenterCircleX, CenterCircleY, Radius + 2, myTFT.C_WHITE);
  myTFT.drawCircle(CenterCircleX, CenterCircleY, Radius + 1, myTFT.C_WHITE);
  myTFT.setCursor(5, 130);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.print("Degrees = ");
  myTFT.setCursor(5, 150);
  myTFT.print("Radians = ");
  myTFT.setCursor(5, 170);
  myTFT.setTextColor(myTFT.C_RED, myTFT.C_BLACK);
  myTFT.print("    Sin = ");
  myTFT.setTextColor(myTFT.C_TAN, myTFT.C_BLACK);
  myTFT.setCursor(5, 190);
  myTFT.print("    Cos = ");
  // Initialize previous needle position at 0 degrees
  int16_t prevX1 = CenterCircleX + (int16_t)(Radius * cos(0));
  int16_t prevY1 = CenterCircleY - (int16_t)(Radius * sin(0));
  int16_t sineX = waveStartX;
  int16_t cosX = waveStartX;
  for (int j = 0; j < 2; j++) {
    for (int angle = 0; angle <= 360; angle++) {
      // Convert angle to radians
      float radians = angle * (PI / 180.0f);
      int16_t x1 = CenterCircleX + (int16_t)(Radius * cos(radians));
      int16_t y1 = CenterCircleY - (int16_t)(Radius * sin(radians));  // Inverted Y for display
      // Erase previous needle
      myTFT.drawLine(CenterCircleX, CenterCircleY, prevX1, prevY1, myTFT.C_BLACK);
      // Draw new needle (counterclockwise)
      myTFT.drawLine(CenterCircleX, CenterCircleY, x1, y1, myTFT.C_GREEN);
      // Update previous needle position
      prevX1 = x1;
      prevY1 = y1;
      // Draw x and y axes
      myTFT.drawFastHLine(0, CenterCircleY, 320, myTFT.C_CYAN);
      myTFT.drawFastVLine(CenterCircleX, 0, 120, myTFT.C_CYAN);
      // Draw sine wave
      int16_t sineY = waveBaseY - (int16_t)(Radius * sin(radians));
      myTFT.drawPixel(sineX, sineY, myTFT.C_RED);
      if (angle % 2) sineX++;
      // Draw cos wave
      int16_t cosY = waveBaseY - (int16_t)(Radius * cos(radians));
      myTFT.drawPixel(cosX, cosY, myTFT.C_TAN);
      if (angle % 2) cosX++;
      myTFT.setCursor(150, 130);
      sprintf(buffer, "%03d", angle);
      myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
      myTFT.print(buffer);
      myTFT.setCursor(150, 150);
      myTFT.print(radians, 4);
      myTFT.setCursor(150, 170);
      myTFT.setTextColor(myTFT.C_RED, myTFT.C_BLACK);
      myTFT.print(sin(radians), 4);
      myTFT.setCursor(150, 190);
      myTFT.setTextColor(myTFT.C_TAN, myTFT.C_BLACK);
      myTFT.print(cos(radians), 4);
      delay(50);
    }
  }
  myTFT.fillScreen(myTFT.C_BLACK);
}

/// @endcond
