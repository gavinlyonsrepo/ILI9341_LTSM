/*! 
	@file    MISC.ino
	@brief   Example file for ili9341 arduino display Library, ILI9341_LTSM, tests Text,graphics & functions.
	@author  Gavin Lyons.
	@details For graphics tests 905-909 to work dislib16_ADVANCED_GRAPHICS_ENABLE must be commented in.
				it is in file display16_common_LTSM from display16_LTSM library.
	@note   See USER OPTIONS 1-2 in SETUP function

	@test 
    -# Test 501 RGB color OK?
    -# Test 502 Scroll
    -# Test 503 Rotate 
    -# Test 504 change modes test -> Invert, display on/off and Sleep.
    -# Test 701 Print out all fonts with writeCharString
    -# Test 702 Print out all fonts with print
    -# Test 703 Misc print class tests(string object, println invert, wrap, base nums etc)
    -# Test 902 rectangles
    -# Test 903 Circle
    -# Test 904 Triangles
    -# Test 905 Polygons
    -# Test 906 Dot Gap
    -# Test 907 Quadrilateral
    -# Test 908 Ellipse
    -# Test 909 Draw Arc
*/

// libraries
#include "ILI9341_LTSM.hpp"
// Included Fonts  : Fonts needed = ALL
#include <fonts_LTSM/FontArialBold_LTSM.hpp>
#include <fonts_LTSM/FontArialRound_LTSM.hpp>
#include <fonts_LTSM/FontDefault_LTSM.hpp>
#include <fonts_LTSM/FontGll_LTSM.hpp>
#include <fonts_LTSM/FontGroTeskBig_LTSM.hpp>
#include <fonts_LTSM/FontGroTesk_LTSM.hpp>
#include <fonts_LTSM/FontHallfetica_LTSM.hpp>
#include <fonts_LTSM/FontInconsola_LTSM.hpp>
#include <fonts_LTSM/FontMega_LTSM.hpp>
#include <fonts_LTSM/FontMint_LTSM.hpp>
#include <fonts_LTSM/FontOrla_LTSM.hpp>
#include <fonts_LTSM/FontPico_LTSM.hpp>
#include <fonts_LTSM/FontRetro_LTSM.hpp>
#include <fonts_LTSM/FontSevenSeg_LTSM.hpp>
#include <fonts_LTSM/FontSinclairS_LTSM.hpp>
#include <fonts_LTSM/FontSixteenSeg_LTSM.hpp>

/// @cond

//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY 1000

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
  myTFT.SetupScreenSize(TFT_WIDTH, TFT_HEIGHT);
  // ===
  myTFT.ILI9341Initialize();
  Serial.println("Start");
  delay(1000);
}

//  Main
void loop(void) {

  Test501();
  Test502();
  Test503();
  Test504();

  Test701();
  Test702();
  Test703();

  Test902();
  Test903();
  Test904();
#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
  Test905();
  Test906();
  Test907();
  Test908();
  Test909();
#endif
  EndTests();
}
// *** End OF MAIN **

void EndTests(void) {
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.PowerDown();  // Power down device
  Serial.println("End");
  while (1) {};
}

void Test501(void) {
  Serial.println("Test 501: Color Test: RGB ,yellow,white, black background");
  myTFT.setFont(FontMega);
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setTextColor(myTFT.C_GREEN, myTFT.C_BLACK);
  myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.C_RED);
  myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.C_GREEN);
  myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.C_BLUE);
  myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.C_YELLOW);
  myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.C_WHITE);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test502(void) {
  Serial.println("Test 502: Scroll");
  myTFT.setFont(FontMega);
  myTFT.setCursor(20, 120);
  myTFT.print("Scroll Test");
  myTFT.setScrollMargins(0, 0);
  for (uint16_t i = 0; i < 100; i++) {
    delay(50);
    myTFT.scrollTo(i);
  }
  delay(TEST_DELAY);
  myTFT.NormalMode();
}

void Test503() {
  Serial.println("Test 503: Rotate");
  myTFT.setFont(FontRetro);
  char teststr0[] = "Rotate 0";    //normal
  char teststr1[] = "Rotate 90";   // 90
  char teststr2[] = "Rotate 180";  // 180
  char teststr3[] = "Rotate 270";  // 270

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_0);
  myTFT.writeCharString(15, 15, teststr0);
  myTFT.writeCharString(15, 300, teststr0);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_90);
  myTFT.writeCharString(15, 15, teststr1);
  myTFT.writeCharString(15, 220, teststr1);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_180);
  myTFT.writeCharString(15, 15, teststr2);
  myTFT.writeCharString(15, 300, teststr2);
  delay(TEST_DELAY2);

  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.setRotation(myTFT.Degrees_270);
  myTFT.writeCharString(15, 15, teststr3);
  myTFT.writeCharString(15, 220, teststr3);
  delay(TEST_DELAY2);

  myTFT.setRotation(myTFT.Degrees_0);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test504() {
  Serial.println("Test 504: Mode Tests");
  char teststr1[] = "Modes Test";
  myTFT.fillRoundRect(8, 10, 24, 60, 8, myTFT.C_RED);
  myTFT.fillRoundRect(32, 10, 24, 60, 8, myTFT.C_GREEN);
  myTFT.fillRoundRect(56, 10, 24, 60, 8, myTFT.C_BLUE);
  myTFT.fillRoundRect(80, 10, 24, 60, 8, myTFT.C_YELLOW);
  myTFT.fillRoundRect(104, 10, 24, 60, 8, myTFT.C_WHITE);
  myTFT.writeCharString(10, 80, teststr1);
  delay(TEST_DELAY2);

  // Invert on and off
  myTFT.invertDisplay(true);
  Serial.println("Test 503-1: Invert on ");
  delay(TEST_DELAY5);
  myTFT.invertDisplay(false);
  Serial.println("Test 503-2: Invert off ");
  delay(TEST_DELAY5);

  // Display on and off
  myTFT.EnableDisplay(false);
  Serial.println("Test 503-3: Display off");
  delay(TEST_DELAY5);
  myTFT.EnableDisplay(true);
  Serial.println("Test 503-4: Turn Display back on");
  delay(TEST_DELAY5);
}

void Test701(void) {

  Serial.println("Test 701: Print out all fonts with writeCharString");
  char teststr1[] = "Default ";
  char teststr2[] = "GLL ";
  char teststr3[] = "Pico ";
  char teststr4[] = "Sinclair ";
  char teststr5[] = "Orla ";
  char teststr6[] = "Retro ";
  char teststr7[] = "Mega";
  char teststr8[] = "Arial b";
  char teststr9[] = "Hall ";
  char teststr10[] = "Arial R";
  char teststr11[] = "GroTesk";

  myTFT.fillScreen(myTFT.C_BLACK);

  myTFT.setFont(FontDefault);
  myTFT.writeCharString(5, 45, teststr1);
  myTFT.setFont(FontGll);
  myTFT.writeCharString(5, 55, teststr2);
  myTFT.setFont(FontPico);
  myTFT.writeCharString(5, 70, teststr3);
  myTFT.setFont(FontSinclairS);
  myTFT.writeCharString(5, 130, teststr4);
  myTFT.setFont(FontOrla);
  myTFT.writeCharString(5, 170, teststr5);
  myTFT.setFont(FontRetro);
  myTFT.writeCharString(5, 200, teststr6);
  DisplayReset();

  myTFT.setFont(FontMega);
  myTFT.writeCharString(5, 52, teststr7);
  myTFT.setFont(FontArialBold);
  myTFT.writeCharString(5, 80, teststr8);
  myTFT.setFont(FontHallfetica);
  myTFT.writeCharString(5, 120, teststr9);
  myTFT.setFont(FontArialRound);
  myTFT.writeCharString(5, 200, teststr10);
  myTFT.setFont(FontGroTesk);
  myTFT.writeCharString(5, 250, teststr11);
  DisplayReset();

  char teststr12[] = "16";
  char teststr13[] = "7";
  char teststr14[] = "inco";
  char teststr15[] = "GB";
  char teststr16[] = "Mint";
  myTFT.setFont(FontSixteenSeg);
  myTFT.writeCharString(5, 5, teststr12);

  myTFT.setFont(FontSevenSeg);
  myTFT.writeCharString(5, 85, teststr13);
  myTFT.setFont(FontInconsola);
  myTFT.writeCharString(5, 180, teststr14);
  DisplayReset();

  myTFT.setFont(FontGroTeskBig);
  myTFT.writeCharString(5, 55, teststr15);
  myTFT.setFont(FontMint);
  myTFT.writeCharString(5, 120, teststr16);
  DisplayReset();
}

void Test702(void) {
  Serial.println("Test 702: Print class methods");
  // Test Fonts default +  + pico+ sinclair + retro
  myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);

  myTFT.setCursor(5, 55);
  myTFT.setFont(FontDefault);
  myTFT.print("Default ");
  myTFT.print(-43);

  myTFT.setCursor(5, 75);
  myTFT.setFont(FontGll);
  myTFT.print("GLL ");
  myTFT.print(123.284, 1);  // print 123.3

  myTFT.setCursor(5, 125);
  myTFT.setFont(FontPico);
  myTFT.print("pico ");
  myTFT.print(747);

  myTFT.setCursor(5, 170);
  myTFT.setFont(FontSinclairS);
  myTFT.print("sinclair ");
  myTFT.print(456);

  myTFT.setCursor(5, 200);
  myTFT.setFont(FontRetro);
  myTFT.print("retro  ");
  myTFT.print(-3.14);
  DisplayReset();

  // Test font  mega Arial bold and Hallf.
  myTFT.setCursor(5, 55);
  myTFT.setFont(FontMega);
  myTFT.println("mega ");
  myTFT.print(61);

  myTFT.setCursor(5, 100);
  myTFT.setFont(FontArialBold);
  myTFT.println("A bold");
  myTFT.print(12.08);

  myTFT.setCursor(5, 160);
  myTFT.setFont(FontHallfetica);
  myTFT.println("hall f");
  myTFT.print(1.48);
  DisplayReset();

  // Test Font orla + arial round

  myTFT.setFont(FontOrla);
  myTFT.setCursor(5, 55);
  myTFT.println(-7.16);
  myTFT.print("Orla");

  myTFT.setCursor(5, 120);
  myTFT.setFont(FontArialRound);
  myTFT.println(-8.16);
  myTFT.print("a rnd");
  DisplayReset();

  // Test font grotesk + sixteen segment
  myTFT.setCursor(5, 75);
  myTFT.setFont(FontGroTesk);
  myTFT.println("GROTESK");
  myTFT.print(1.78);
  myTFT.setCursor(0, 180);
  myTFT.setFont(FontSixteenSeg);
  myTFT.print(1245);
  DisplayReset();

  // Test font seven segment
  myTFT.setCursor(5, 55);
  myTFT.setFont(FontSevenSeg);
  myTFT.println(12);
  DisplayReset();

  myTFT.setFont(FontMint);
  myTFT.println("MINT");
  ;
  myTFT.print("9`C");
  DisplayReset();
}

void Test703(void) {
  Serial.println("Test 703: Misc print class(println invert, wrap, base nums etc)");
  //Inverted print
  myTFT.setTextColor(myTFT.C_RED, myTFT.C_YELLOW);
  myTFT.setFont(FontDefault);

  myTFT.setFont(FontSinclairS);
  myTFT.setCursor(5, 55);
  myTFT.print(-49);

  myTFT.setFont(FontMega);
  myTFT.setCursor(5, 90);
  myTFT.print(112.09);
  myTFT.setCursor(5, 120);
  myTFT.print("ABCD");

  myTFT.setFont(FontGll);
  myTFT.setCursor(5, 200);
  myTFT.print("ER");

  DisplayReset();

  // Inverted print
  myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_RED);

  myTFT.setFont(FontArialBold);
  myTFT.setCursor(5, 55);
  myTFT.print("INVERT");
  myTFT.setCursor(5, 85);
  myTFT.print(-94.982, 2);

  myTFT.setFont(FontHallfetica);
  myTFT.setCursor(5, 120);
  myTFT.print("INVERT");
  myTFT.setCursor(5, 200);
  myTFT.print(123456);
  DisplayReset();

  // Test print with DEC BIN OCT HEX
  myTFT.setTextColor(myTFT.C_WHITE, myTFT.C_BLACK);
  uint8_t numPos = 47;
  myTFT.setFont(FontMega);
  myTFT.setCursor(5, 55);
  myTFT.print(numPos, DEC);  // 47
  myTFT.setCursor(5, 75);
  myTFT.print(numPos, BIN);  // 10111
  myTFT.setCursor(5, 120);
  myTFT.print(numPos, OCT);  // 57
  myTFT.setCursor(5, 160);
  myTFT.print(numPos, HEX);  // 2F

  DisplayReset();
  //text wrap with print
  myTFT.setCursor(5, 70);
  myTFT.print("12345678901234567890ABCDEFGHIJ");
  DisplayReset();
}

void Test902(void) {
  Serial.println("Test 902: rectangles ");
  DisLib16::Ret_Codes_e returnValue;
  myTFT.drawRectWH(25, 25, 20, 20, myTFT.C_RED);
  returnValue = myTFT.fillRectBuffer(85, 25, 20, 20, myTFT.C_YELLOW);
  if (returnValue != DisLib16::Success) {
    Serial.print("Warning : Test TFTfillRectangle, An error occurred returnValue = ");
    Serial.println(returnValue);
  }
  myTFT.fillRect(115, 25, 20, 20, myTFT.C_GREEN);
  myTFT.drawRoundRect(15, 100, 70, 70, 20, myTFT.C_CYAN);
  myTFT.fillRoundRect(120, 100, 50, 50, 10, myTFT.C_WHITE);

  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test903(void) {
  Serial.println("Test 903 & 904 : Triangles and circles");
  myTFT.drawCircle(40, 20, 15, myTFT.C_GREEN);
  myTFT.fillCircle(80, 20, 15, myTFT.C_YELLOW);
}

void Test904(void) {
  myTFT.drawTriangle(5, 80, 50, 40, 95, 80, myTFT.C_CYAN);
  myTFT.fillTriangle(55, 120, 100, 90, 127, 120, myTFT.C_RED);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

#ifdef dislib16_ADVANCED_GRAPHICS_ENABLE
void Test905(void) {
  Serial.println("Test 905 : polygons");
  myTFT.fillScreen(myTFT.C_BLACK);
  //drawPolygon( x,  y, sides,  diameter,  rotation, color);
  // decagon
  myTFT.drawPolygon(50, 100, 10, 50, 0, false, myTFT.C_RED);
  // octagon
  myTFT.drawPolygon(150, 100, 8, 50, 0, false, myTFT.C_GREEN);
  // hexagon rotated
  myTFT.drawPolygon(50, 200, 6, 50, 45, false, myTFT.C_WHITE);
  // pentagon
  myTFT.drawPolygon(150, 200, 5, 50, 0, false, myTFT.C_TAN);
  delay(TEST_DELAY5);
  // decagon
  myTFT.drawPolygon(50, 100, 10, 50, 0, true, myTFT.C_RED);
  // octagon
  myTFT.drawPolygon(150, 100, 8, 50, 0, true, myTFT.C_GREEN);
  // hexagon rotated
  myTFT.drawPolygon(50, 200, 6, 50, 45, true, myTFT.C_WHITE);
  // pentagon
  myTFT.drawPolygon(150, 200, 5, 50, 0, true, myTFT.C_TAN);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test906(void) {
  Serial.println("Test 906 : mesh");
  myTFT.fillScreen(myTFT.C_BLACK);
  //drawDotGrid(x, y,  w,  h,  DotGridGap, color);
  myTFT.drawDotGrid(0, 0, 100, 100, 5, myTFT.C_CYAN);
  myTFT.drawDotGrid(0, 200, 100, 100, 10, myTFT.C_GREEN);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test907() {
  Serial.println("Test 907 : quadrilateral");
  // Define the four quadrilateral vertices for different shapes
  // First shape (quadrilateral )
  int16_t x0 = 10, y0 = 10;  // First vertex
  int16_t x1 = 50, y1 = 10;
  int16_t x2 = 50, y2 = 50;
  int16_t x3 = 10, y3 = 50;
  // Second shape (Parallelogram )
  int16_t x4 = 180, y4 = 240;  // First vertex
  int16_t x5 = 230, y5 = 250;
  int16_t x6 = 220, y6 = 290;
  int16_t x7 = 170, y7 = 280;
  // Third shape (Trapezoid 1)
  int16_t x8 = 106, y8 = 107;  // First vertex
  int16_t x9 = 112, y9 = 128;
  int16_t x10 = 134, y10 = 128;
  int16_t x11 = 153, y11 = 107;
  // Fourth shape (Trapezoid 2)
  int16_t x12 = 16, y12 = 207;  // First vertex
  int16_t x13 = 29, y13 = 242;
  int16_t x14 = 85, y14 = 242;
  int16_t x15 = 46, y15 = 207;

  myTFT.drawQuadrilateral(x0, y0, x1, y1, x2, y2, x3, y3, myTFT.C_RED);
  myTFT.fillQuadrilateral(x4, y4, x5, y5, x6, y6, x7, y7, myTFT.C_GREEN);
  myTFT.fillQuadrilateral(x8, y8, x9, y9, x10, y10, x11, y11, myTFT.C_BLUE);
  myTFT.drawQuadrilateral(x12, y12, x13, y13, x14, y14, x15, y15, myTFT.C_YELLOW);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test908(void) {
  Serial.println("Test 908 : Ellipse");
  myTFT.fillScreen(myTFT.C_BLACK);
  // Define ellipses at different positions
  // Format: drawEllipse(centerX, centerY, semiMajorAxis, semiMinorAxis, fill, color)
  myTFT.drawEllipse(TFT_WIDTH / 4, TFT_HEIGHT / 4, 20, 40, false, myTFT.C_WHITE);           // Top-left
  myTFT.drawEllipse(3 * TFT_WIDTH / 4, TFT_HEIGHT / 4, 25, 50, false, myTFT.C_RED);         // Top-right
  myTFT.drawEllipse(TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 60, 30, false, myTFT.C_BLUE);        // Bottom-left
  myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, false, myTFT.C_YELLOW);  // Bottom-right
  delay(TEST_DELAY5);
  myTFT.drawEllipse(TFT_WIDTH / 4, TFT_HEIGHT / 4, 20, 40, true, myTFT.C_WHITE);
  myTFT.drawEllipse(3 * TFT_WIDTH / 4, TFT_HEIGHT / 4, 25, 50, true, myTFT.C_RED);
  myTFT.drawEllipse(TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 60, 30, true, myTFT.C_BLUE);
  myTFT.drawEllipse(3 * TFT_WIDTH / 4, 3 * TFT_HEIGHT / 4, 50, 35, true, myTFT.C_YELLOW);
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}

void Test909(void) {
  Serial.println("Test 909 : Drawing Arc: drawArc");
  myTFT.fillScreen(myTFT.C_BLACK);
  int16_t centerX = 120;  // X-coordinate of the circle center
  int16_t centerY = 160;  // Y-coordinate of the circle center
  int16_t radius = 50;    // Radius of the circle
  Serial.print(" Angle offset default : ");
  Serial.println(myTFT.getArcAngleOffset());
  //drawArc(cx,  cy,  radius, thickness, startAngle, endAngle, color);
  // _arcAngleOffset is zero default means 0 is positive Axis , Arc drawn counterclockwise
  // XXX 270 XXX
  // 180 XXX 000
  // XXX 090 XXX
  // Draw the full lower half (0° to 180°)
  myTFT.drawArc(centerX, centerY, radius, 7, 0.0f, 180.0f, myTFT.C_GREEN);
  delay(TEST_DELAY2);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the full upper half (180° to 360°)
  myTFT.drawArc(centerX, centerY, radius, 10, 180.0f, 360.0f, myTFT.C_BLUE);
  delay(TEST_DELAY2);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the first quarter (0° to 90°)
  myTFT.drawArc(centerX, centerY, radius, 2, 0.0f, 90.0f, myTFT.C_RED);
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the second quarter (90° to 180°)
  myTFT.drawArc(centerX, centerY, radius, 2, 90.0f, 180.0f, myTFT.C_YELLOW);
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the third quarter (180° to 270°)
  myTFT.drawArc(centerX, centerY, radius, 2, 180.0f, 270.0f, myTFT.C_CYAN);
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
  // Draw the fourth quarter (270° to 360°)
  myTFT.drawArc(centerX, centerY, radius, 2, 270.0f, 360.0f, myTFT.C_MAGENTA);
  delay(TEST_DELAY);
  myTFT.fillScreen(myTFT.C_BLACK);
}

#endif

void DisplayReset(void) {
  delay(TEST_DELAY5);
  myTFT.fillScreen(myTFT.C_BLACK);
}


/// @endcond
