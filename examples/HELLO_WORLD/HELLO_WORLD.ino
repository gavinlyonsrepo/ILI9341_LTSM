/*! 
	@file   HELLO_WORLD.ino
	@brief  Hello World basic use case, ili9341 controller , ILI9341_LTSM arduino library
	@author Gavin Lyons.
	@note   See USER OPTIONS 1-2 in SETUP function
	@test 
		-# 101 Hello World
*/
// libraries
#include "ILI9341_LTSM.hpp"
// Fonts needed
#include "fonts_LTSM/FontGroTesk_LTSM.hpp"

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
}

void loop(void) {
  HelloWorld();
  EndTests();
}

void HelloWorld(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.setFont(FontGroTesk);
  myTFT.setCursor(0, 15);
  myTFT.print("Hello World");
  delay(7000);
}

void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.PowerDown();  // Power down device
  Serial.println("End");
  while(1){};
}

// *************** EOF ****************

/// @endcond
