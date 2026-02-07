/*!
	@file    DEMO_THREE.ino
	@author  Gavin Lyons
	@brief   Example file for ili4391 library.  ILI4931_LTSM library. demo
	@details See USER OPTIONS 1-2 in SETUP function
	@note  Tested only on ESP32 240Mhz
	@test
		-# Test 454 draw Mandelbrot set benchmark, 120x160 pixels, (time taken 253 mS, using a buffer)
*/

// libraries
#include "ILI9341_LTSM.hpp"

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
}

// MAIN loop
void loop(void) {
  drawMandelbrot();
  EndTests();
}
// *** End OF MAIN **

//  Function Space

void EndTests(void) {
  myTFT.PowerDown();  // Power down device
  Serial.println("Demo Over");
  while (1) {};
}

void drawMandelbrot(void) {
  myTFT.setRotation(myTFT.Degrees_90);
  Serial.println("Start Test");
  uint32_t startTime, elapsedTime;  // Variables to track start and elapsed time

  // Constants for image and fractal settings
  const int16_t bits = 20;          // Fractional resolution used for precision (higher bits = finer precision)
  const int16_t pixelWidth = 160;   // Width of drawing in pixels
  const int16_t pixelHeight = 120;  // Height of drawing in pixels
  const int16_t iterations = 128;   // Maximum number of iterations to check for each pixel
  // Mandelbrot set properties: Center point and coverage area in the complex plane
  float centerReal = -0.6;  // Image center point in complex plane
  float centerImag = 0.0;   // Image center point in complex plane
  float rangeReal = 3.0;    // Image coverage in complex plane
  float rangeImag = 3.0;    // Image coverage in complex plane

  // Buffer to store pixel color values (16-bit per pixel)
  uint8_t* buffer = (uint8_t*)malloc(pixelWidth * pixelHeight * 2);
  if (!buffer) {
    Serial.println("Buffer alloc failed!");
    return;
  }

  int64_t n, a, b, a2, b2, posReal, posImag;
  int32_t startReal = (int64_t)((centerReal - rangeReal * 0.5) * (float)(1 << bits));
  int32_t startImag = (int64_t)((centerImag + rangeImag * 0.5) * (float)(1 << bits));
  int32_t incReal = (int64_t)((rangeReal / (float)pixelWidth) * (float)(1 << bits));
  int32_t incImag = (int64_t)((rangeImag / (float)pixelHeight) * (float)(1 << bits));

  startTime = millis();  // Start Timer Calculation

  posImag = startImag;
  for (int y = 0; y < pixelHeight; y++) {
    posReal = startReal;
    for (int x = 0; x < pixelWidth; x++) {
      a = posReal;
      b = posImag;
      for (n = iterations; n > 0; n--) {
        a2 = (a * a) >> bits;
        b2 = (b * b) >> bits;
        if ((a2 + b2) >= (4 << bits))
          break;
        b = posImag + ((a * b) >> (bits - 1));
        a = posReal + a2 - b2;
      }
      // Calculate the color (16-bit value)
      uint16_t color = (n * 29) << 8 | (n * 67);

      // Write to the buffer (split into high and low bytes)
      buffer[2 * (y * pixelWidth + x)] = (color >> 8) & 0xFF;  // High byte (most significant byte)
      buffer[2 * (y * pixelWidth + x) + 1] = color & 0xFF;     // Low byte (least significant byte)
      posReal += incReal;
    }
    posImag -= incImag;
  }

  myTFT.drawBitmap16Data(0, 0, buffer, pixelWidth, pixelHeight);

  elapsedTime = millis() - startTime;  // End Timer Calculation
  Serial.print("Time Taken in mS :: ");
  Serial.println(elapsedTime);
  Serial.println("End Test");
  delay(5000);
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_0);
}

/// @endcond
