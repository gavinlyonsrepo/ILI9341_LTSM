/*! 
	@file    READ_DIAG.ino
	@brief   Example file for ili9341 arduino display Library, ILI9341_LTSM, 
				read diagnostics from Display. Useful for diagnostics, debug and testing.
	@author  Gavin Lyons.
	@details ILI9341 Read Diagnostics, Results from author's display :
			-# Display Power Mode: 0x94
			-# MADCTL Mode: 0x48
			-# Pixel Format: 0x05
			-# Image Format: 0x80
			-# Self Diagnostic: 0xC0
	@note   See USER OPTIONS 1-2 in SETUP function, An extra pin 'MISO' is required for this example,
			to read data back from Display. In Software SPI pick any GPIO , Hardware tied to MCU SPI
			interface
	@test 
	-# Test 500 Read Diagnostics from display
*/

// libraries
#include "ILI9341_LTSM.hpp"
// Included Fonts  : Fonts needed = ALL
#include <fonts_LTSM/FontArialBold_LTSM.hpp>

/// @cond

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

uint16_t TFT_WIDTH = 240;   // Screen width in pixels
uint16_t TFT_HEIGHT = 320;  // Screen height in pixels
ILI9341_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

void setup(void) {
  Serial.begin(38400);
  delay(1000);
  Serial.println("Start");
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 5;
  int8_t RST_TFT = 4;
  int8_t CS_TFT = 15;
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
    myTFT.SetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
    Serial.println("HW SPI");
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_TFT = 13;
    int8_t SCLK_TFT = 12;
    int8_t MISO_TFT = 14;
    myTFT.SetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT, MISO_TFT);
    Serial.println("SW SPI");
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  myTFT.SetupScreenSize(TFT_WIDTH, TFT_HEIGHT);
  // ===
  myTFT.ILI9341Initialize();
}

//  Main
void loop(void) {
  myTFT.fillScreen(myTFT.C_RED);
  myTFT.setFont(FontArialBold);
  myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);
  myTFT.print("Read Diagnostic  output to    serial monitor");
  delay(3000);
  printDiagnostic();
  EndTests();
}
// *** End OF MAIN **

void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.PowerDown();  // Power down device
  Serial.println("End");
  while (1) {};
}

void printDiagnostic(void) {
  Serial.println(F("==== ILI9341 Diagnostics ===="));
  uint8_t returnByte;

  //  following comment block not implented.
  /*
  // Display ID (3 bytes) : Not supported on many ILI9341 panels (MISO not wired, returns 0x00/0xFF):
  Serial.print(F("Display ID Info: "));
  for (uint8_t i = 0; i < 3; i++) {
    returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDID, i);
    Serial.print(F("0x"));
    Serial.print(returnByte, HEX);
    Serial.print(F(" "));
  }
  Serial.println();

  // Display Status (4 bytes) :  Same issue as above, status reads back junk (0xD2 repeated):
  Serial.print(F("Read Display Status: "));
  for (uint8_t i = 0; i < 4; i++) {
    returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDSTATUS, i);
    Serial.print(F("0x"));
    Serial.print(returnByte, HEX);
    Serial.print(F(" "));
  }
  Serial.println();
  // Usually not implemented, always returns 0x00:
  returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDSIGNAL, 0);
  Serial.print(F("Display Signal Mode: 0x"));
  Serial.println(returnByte, HEX);
*/

  returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDMODE, 0);
  Serial.print(F("Display Power Mode: 0x"));
  Serial.println(returnByte, HEX);

  returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDMADCTL, 0);
  Serial.print(F("MADCTL Mode: 0x"));
  Serial.println(returnByte, HEX);

  returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDPIXFMT, 0);
  Serial.print(F("Pixel Format: 0x"));
  Serial.println(returnByte, HEX);

  returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDIMGFMT, 0);
  Serial.print(F("Image Format: 0x"));
  Serial.println(returnByte, HEX);

  returnByte = myTFT.readDiagByte(myTFT.ILI9341_RDSELFDIAG, 0);
  Serial.print(F("Self Diagnostic: 0x"));
  Serial.println(returnByte, HEX);
}



/// @endcond
