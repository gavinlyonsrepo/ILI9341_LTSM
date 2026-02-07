/*!
	@file  DEMO_FOUR.ino
	@brief Example file for ili9341 arduino display Library, ILI9341_LTSM. Demo tested on ESP32.
	@note  See USER OPTIONS 1-3 in SETUP function
	@test
		-# test 405 Control panel demo
*/

// Section ::  libraries
//#include <ctime>
#include <random> // gauge
#include <algorithm> // For std::clamp
#include <limits> // menu limits


#include "ILI9341_LTSM.hpp"
#include <fonts_LTSM/FontDefault_LTSM.hpp>

/// @cond
#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif
#ifndef dislib16_ADVANCED_GRAPHICS_ENABLE
#pragma message("gll: dislib16_ADVANCED_GRAPHICS_ENABLE is not defined. it is required for this demo")
#endif

// Section :: Globals
//  Test timing related defines
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY 1000

#ifdef dislib16_ADVANCED_SCREEN_BUFFER_ENABLE
#pragma message("gll: dislib16_ADVANCED_SCREEN_BUFFER_ENABLE is defined. This example is not for that mode")
#endif

uint16_t SCREEN_WIDTH = 240;   // Screen width in pixels
uint16_t SCREEN_HEIGHT = 320;  // Screen height in pixels
ILI9341_LTSM myTFT;
bool bhardwareSPI = true;  // true for hardware spi, false for software

//  Section ::  Function Headers
// Demo 5
const uint16_t QUAD_WIDTH = SCREEN_WIDTH / 2;  // 120px
const uint16_t QUAD_HEIGHT = SCREEN_HEIGHT / 2; // 160px
void ControlPanel(uint16_t countLimit = 100);
// Demo 5-1
void gaugeDemo(uint16_t count = 0);
void drawPointerHelper(int16_t val, uint8_t x, uint8_t y, uint8_t r, uint16_t color);
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor);
void drawPointer(int16_t &val, int16_t &oldVal , uint8_t x, uint8_t y, uint8_t r, uint16_t color, uint16_t bcolor);
// Demo 5-2
void arcGauge(uint16_t count = 0);
// Demo 5-3
// Gauge parameters  (Q3)
const int GAUGE_WIDTH = 18;
const int GAUGE_HEIGHT = 70;
const int GAUGE_X_START = 7;
const int GAUGE_Y_START = 220;
const int GAUGE_SPACING = 38;
// Store previous values to prevent redundant redraws
float prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;
void updateGauges(float phase);
void drawGauge(int x, int y, uint16_t color, float value);
void drawGaugeDemo(uint16_t  count = 0);
void drawGaugeBorder(int x, int y);
// Demo 5-4
// VU Meter parameters (Q4)
const uint16_t VU_X = QUAD_WIDTH + 10;
const uint16_t VU_Y = QUAD_HEIGHT + 60;
const uint16_t VU_BAR_WIDTH = 12;
const uint16_t VU_BAR_HEIGHT = 70;
const uint8_t VU_BAR_COUNT = 5;
const uint16_t VU_BAR_SPACING = 5;
float vuValues[VU_BAR_COUNT] = {0};
void vuMeter(uint16_t count = 0);

void setup(void) {
  Serial.begin(38400);
  delay(1000);
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 5;
  int8_t RST_TFT = 4;
  int8_t CS_TFT = 15;
  if (bhardwareSPI == true) {          // hw spi
    uint32_t TFT_SCLK_FREQ = 24000000;  // Spi freq in Hertz
    myTFT.SetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  } else {                        // sw spi
    uint16_t SWSPICommDelay = 0;  // optional SW SPI GPIO delay in uS
    int8_t SDIN_TFT = 13;
    int8_t SCLK_TFT = 12;
    myTFT.SetupGPIO_SPI(SWSPICommDelay, RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
  }
  // ===
  // === USER OPTION 2 Screen Setup ===
  myTFT.SetupScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  // ===
  myTFT.ILI9341Initialize();
  Serial.println("Start");
  delay(1000);
}

//  Section ::  MAIN loop
void loop()
{
	ControlPanel(100);
	EndTests();
}
// *** End OF MAIN **


//  Section ::  Function Space


// --- Utilities + Misc functions ---
void EndTests(void)
{
  myTFT.fillScreen(myTFT.C_BLACK);
  myTFT.PowerDown();  // Power down device
  Serial.println("End");
  while (1) {};
}

// Demo 5

void ControlPanel(uint16_t countLimit)
{
	Serial.print("Demo 5 ends at: ");
	Serial.println(countLimit);
	// setup
	myTFT.setRotation(myTFT.Degrees_0);
	uint16_t count = 0;
	myTFT.fillScreen(myTFT.C_BLACK);
	myTFT.setFont(FontDefault);
	myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
	// Draw divider lines
	myTFT.drawLine(QUAD_WIDTH, 0, QUAD_WIDTH, SCREEN_HEIGHT, myTFT.C_GREY); // Vertical
	myTFT.drawLine(0, QUAD_HEIGHT, SCREEN_WIDTH, QUAD_HEIGHT, myTFT.C_GREY); // Horizontal
	myTFT.drawRectWH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, myTFT.C_GREY); // Border

	while (count < countLimit) {
		gaugeDemo(count);      // Q1: Top-Left
		arcGauge(count);       // Q2: Top-Right
		drawGaugeDemo(count);  // Q3: Bottom-Left
		vuMeter(count);        // Q4: Bottom-Right
		count++;
	}
	myTFT.fillScreen(myTFT.C_BLACK);
	Serial.println("Demo 5 Over");
}

// Demo 5-1
void gaugeDemo(uint16_t count)
{
	static int16_t currentValue = 150;
	static int16_t oldValue = 150;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	uint8_t x = 50;
	uint8_t y = 50;
	uint8_t radius = 40;
	char buffer[10];
	// Draw Gauge on first pass
	if(count == 0 )
	{
		myTFT.setCursor(10,100);
		myTFT.print("RPM  :");
		myTFT.drawCircle(x, y, radius, myTFT.C_RED);
		drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
		drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
	}
	// Random number setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes

	int step = stepDist(gen);
	currentValue += step;
	currentValue = std::clamp(currentValue, minValue, maxValue);
	if (oldValue != currentValue) {
		drawPointer(currentValue, oldValue,  x, y, radius, myTFT.C_GREEN, myTFT.C_BLACK);
		oldValue = currentValue;
	}
	myTFT.setCursor(75,100);
	sprintf(buffer, "%03d", currentValue);
	myTFT.print(buffer);
}

// Demo 5-1 & Demo 5-2
void drawGaugeMarkers(uint8_t centerX, uint8_t centerY, uint8_t radius, int startAngle, int endAngle, float scaleFactor)
{
	float angleRad, innerX, innerY, outerX, outerY;
	int angle;
	// Loop through the specified angle range, drawing ticks every 30 degrees
	for (angle = startAngle; angle <= endAngle; angle += 30)
	{
		// Convert degrees to radians
		angleRad = angle * (std::numbers::pi / 180);
		// inner marker position
		innerX = cos(angleRad) * (radius / scaleFactor);
		innerY = sin(angleRad) * (radius / scaleFactor);
		// outer marker position
		outerX = cos(angleRad) * radius;
		outerY = sin(angleRad) * radius;
		// Draw marker line from inner to outer radius
		myTFT.drawLine(1 + centerX + innerX, 1 + centerY + innerY,
						 1 + centerX + outerX, 1 + centerY + outerY,
						 myTFT.C_WHITE);
	}
}

// Demo 5-1 & Demo 5-2
void drawPointer(int16_t &currentValue, int16_t &oldValue, uint8_t x, uint8_t y, uint8_t r, uint16_t colour, uint16_t bcolour)
{
	uint16_t i;
	// If the current value is increasing
	if (currentValue > oldValue)
	{
		// Incrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i <= currentValue; i++)
		{
			// Erase the previous pointer position
			drawPointerHelper(i - 1, x, y, r, bcolour);
			// Draw the pointer at the new position
			drawPointerHelper(i, x, y, r, colour);
			// Introduce a small delay for smooth animation
			if ((currentValue - oldValue) < 128) {
				delay(20);
			}else{
				delay(30); // Slower update for small changes
			}
		}
	}
	else // If the current value is decreasing
	{
		// Decrementally move the pointer from oldValue to currentValue
		for (i = oldValue; i >= currentValue; i--)
		{
			// Erase the previous pointer position
			drawPointerHelper(i + 1, x, y, r, bcolour);
			// Draw the pointer at the new position
			drawPointerHelper(i, x, y, r, colour);
			// Adjust delay based on the difference to control animation speed
			if ((oldValue - currentValue) >= 128) {
				delay(20); // Faster update for large changes
			}else{
				delay(30); // Slower update for small changes
			}
		}
	}
}

//  Demo 5-1 & Demo 5-2
void drawPointerHelper(int16_t value, uint8_t centerX, uint8_t centerY, uint8_t radius, uint16_t pointerColor)
{
	float angleRad, pointerX, pointerY;
	int16_t minValue = 0;
	int16_t maxValue = 255;
	int startAngle = 150; // Gauge starting angle in degrees
	int endAngle = 240;   // Gauge ending angle in degrees
	// Ensure the value stays within the valid range
	if (value > maxValue) value = maxValue;
	if (value < minValue) value = minValue;
	// Map value to an angle in radians based on the gauge range
	angleRad = (((float)(uint16_t)(value - minValue) / (float)(uint16_t)(maxValue - minValue) * endAngle)
				 + startAngle) * (std::numbers::pi / 180);
	// Calculate pointer tip coordinates
	pointerX = cos(angleRad) * (radius / 1.35);
	pointerY = sin(angleRad) * (radius / 1.35);
	// Draw the pointer line from the center to the calculated tip position
	myTFT.drawLine(centerX, centerY, 1 + centerX + pointerX, 1 + centerY + pointerY, pointerColor);
	// Draw a small circle to represent the pointer base
	myTFT.fillCircle(centerX, centerY, 2, pointerColor);
}

// Demo 5-2
void arcGauge(uint16_t count)
{
	static int16_t currentValue = 150;
	static int16_t oldValue = 150;
	char buffer[10];
	// Draw Gauge
	uint8_t x = 170;
	uint8_t y = 60;
	uint8_t radius = 40;
	const int16_t minValue = 1;
	const int16_t maxValue = 255;
	if (count == 0){
		myTFT.drawArc(x, y, radius, 10,  150.0f, 180.0f, myTFT.C_RED);
		myTFT.drawArc(x, y, radius, 10,  181.0f, 250.0f, myTFT.C_ORANGE);
		myTFT.drawArc(x, y, radius, 10,  251.0f, 320.0f, myTFT.C_YELLOW);
		myTFT.drawArc(x, y, radius, 10,  321.0f, 35.0f, myTFT.C_GREEN);
		drawGaugeMarkers(x, y, radius, 150, 390, 1.3);
		drawGaugeMarkers(x, y, radius, 165, 375, 1.1);
		myTFT.setCursor(130,100);
		myTFT.print("TEMP :");
	}
	// Random number setup
	// Initialize random generator
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> stepDist(-10, 10); // Small step changes
	int step = stepDist(gen);
	currentValue += step;
	currentValue = std::clamp(currentValue, minValue, maxValue);
	myTFT.setCursor(0,100);
	if (oldValue != currentValue) {
		drawPointer(currentValue, oldValue,  x, y, radius, myTFT.C_GREEN, myTFT.C_BLACK);
		oldValue = currentValue;
	}
	sprintf(buffer, "%03d", currentValue);
	myTFT.setCursor(200,100);
	myTFT.print(buffer);

}

// Demo 5-3
void drawGaugeBorder(int x, int y) { // Draw border once
	myTFT.drawRectWH(x - 2, y - 2, GAUGE_WIDTH + 4, GAUGE_HEIGHT + 4, myTFT.C_WHITE);
}

void drawGaugeDemo(uint16_t count)
{
	// Draw borders once
	if (count == 0){

		myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START - GAUGE_HEIGHT + 45);
		myTFT.print("VOL");
		drawGaugeBorder(GAUGE_X_START, GAUGE_Y_START);

		myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START - GAUGE_HEIGHT + 45);
		myTFT.print("LVL");
		drawGaugeBorder(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START);

		myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START - GAUGE_HEIGHT + 45);
		myTFT.print("PWR");
		drawGaugeBorder(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START);

		prevVal1 = -1, prevVal2 = -1, prevVal3 = -1;
	}
	static float phase = 0;
	updateGauges(phase);
	phase += 0.1;
}

// Gauge update
void drawGauge(int x, int y, float value, float prevVal) {
	int fillHeight = static_cast<int>(GAUGE_HEIGHT * value);
	int prevFillHeight = (prevVal < 0) ? 0 : static_cast<int>(GAUGE_HEIGHT * prevVal);

	if (fillHeight == prevFillHeight) return;  // nothing to update

	// If shrinking, clear the difference
	if (fillHeight < prevFillHeight) {
	int clearHeight = prevFillHeight - fillHeight;
	myTFT.fillRectBuffer(
		x,
		y + (GAUGE_HEIGHT - prevFillHeight),
		GAUGE_WIDTH,
		clearHeight,
		myTFT.C_BLACK);
	}
	// If growing, draw gradient for the new part
	if (fillHeight > prevFillHeight) {
	int growHeight = fillHeight - prevFillHeight;
	for (int i = 0; i < growHeight; i++) {
		uint8_t val = map(static_cast<int>(prevFillHeight + i), 0, GAUGE_HEIGHT - 1, 1, 127);
		uint16_t color = generateColor(val);
		myTFT.fillRectBuffer(
		x,
		y + (GAUGE_HEIGHT - fillHeight) + (growHeight - 1 - i),
		GAUGE_WIDTH,
		1,
		color);
	}
	}
}

// Update all gauges + numeric readouts
void updateGauges(float phase) {
	// Calculate values (0 → 1)
	float val1 = (std::sin(phase) + 1) / 2;
	float val2 = (std::cos(phase) + 1) / 2;
	float val3 = ((std::sin(phase) * std::cos(phase)) + 1) / 2;

	char buffer[6];  // formatted text
	// --- Gauge 1 ---
	if (val1 != prevVal1) {
		drawGauge(GAUGE_X_START, GAUGE_Y_START, val1, prevVal1);
		sprintf(buffer, "%.2f", val1);
		myTFT.fillRectBuffer(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK);
		myTFT.setCursor(GAUGE_X_START, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal1 = val1;
	}
	// --- Gauge 2 ---
	if (val2 != prevVal2) {
		drawGauge(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START, val2, prevVal2);
		sprintf(buffer, "%.2f", val2);
		myTFT.fillRectBuffer(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK);
		myTFT.setCursor(GAUGE_X_START + GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal2 = val2;
	}
	// --- Gauge 3 ---
	if (val3 != prevVal3) {
		drawGauge(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START, val3, prevVal3);
		sprintf(buffer, "%.2f", val3);
		myTFT.fillRectBuffer(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10, 36, 8, myTFT.C_BLACK);
		myTFT.setCursor(GAUGE_X_START + 2 * GAUGE_SPACING, GAUGE_Y_START + GAUGE_HEIGHT + 10);
		myTFT.print(buffer);
		prevVal3 = val3;
	}
}

// Demo 5-4
// Q4: VU Meter (Bottom-Right)
void vuMeter(uint16_t count)
{
	// Throttle update rate: update every 2nd frame
	if (count & 0x01) return;

	static uint16_t prevHeight[VU_BAR_COUNT] = {0};

	if (count == 0) {// Draw label and bar outlines once at start
		myTFT.setFont(FontDefault);
		myTFT.setTextColor(myTFT.C_YELLOW, myTFT.C_BLACK);
		myTFT.setCursor(QUAD_WIDTH + 10, QUAD_HEIGHT + 140);
		myTFT.print("SIG:");
		for (int i = 0; i < VU_BAR_COUNT; i++) {
			int16_t x = VU_X + i * (VU_BAR_WIDTH + VU_BAR_SPACING);
			myTFT.drawRectWH(x, VU_Y, VU_BAR_WIDTH, VU_BAR_HEIGHT, myTFT.C_WHITE);
		}
	}
	// Fake data 
	float t = count * 0.1f;
	float sum = 0.0f;
	for (int i = 0; i < VU_BAR_COUNT; i++) {
		vuValues[i] = 50.0f
			+ 30.0f * sin(t * (0.8f + i * 0.1f))
			+ (rand() % 20 - 10);
		if (vuValues[i] < 0) vuValues[i] = 0;
		if (vuValues[i] > 100) vuValues[i] = 100;
		sum += vuValues[i];
	}

	int avgValue = (int)(sum / VU_BAR_COUNT);
	myTFT.fillRectBuffer(QUAD_WIDTH + 46, QUAD_HEIGHT + 140, 20, 10, myTFT.C_BLACK);
	myTFT.setCursor(QUAD_WIDTH + 46, QUAD_HEIGHT + 140);
	myTFT.print(avgValue);
	// Draw bars using delta updates
	for (int i = 0; i < VU_BAR_COUNT; i++) {
		int16_t x = VU_X + i * (VU_BAR_WIDTH + VU_BAR_SPACING);
		uint16_t newHeight =
			(uint16_t)(VU_BAR_HEIGHT * (vuValues[i] / 100.0f));
		if (newHeight > prevHeight[i]) {
			// Draw only added section
			uint8_t colorVal = 1 + ((uint16_t)vuValues[i] * 126) / 100;
			uint16_t barColor = myTFT.C_GREEN;
			myTFT.fillRectBuffer(
				x + 2,
				VU_Y + (VU_BAR_HEIGHT - newHeight),
				VU_BAR_WIDTH - 4,
				newHeight - prevHeight[i],
				barColor
			);
		}
		else if (newHeight < prevHeight[i]) {
			// Clear only removed section
			myTFT.fillRectBuffer(
				x + 2,
				VU_Y + (VU_BAR_HEIGHT - prevHeight[i]),
				VU_BAR_WIDTH - 4,
				prevHeight[i] - newHeight,
				myTFT.C_BLACK
			);
		}
		prevHeight[i] = newHeight;
	}
}

/*!
	@brief This helper function is designed to return a color in the 16-bit 
		RGB format based on the input value, creating a smooth transition 
		between colors in different stages.
	@param value input value 1-127
	@return 16 bit color
	@details The gradient is calculated by 
		cycling through different color transitions as val increases, 
		allowing for smooth visual effects such as changing hues 
		or creating rainbow-like effects on an LCD. 1-127 will
		This will create a gradient color between blue and red based on the value.
*/
uint16_t generateColor(uint8_t value)
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t segment = value / 32;
	switch(segment) 
	{
		case 0: red = 0; green = 2 * (value % 32); blue = 31; break;
		case 1: red = 0; green = 63; blue = 31 - (value % 32); break;
		case 2: red = value % 32; green = 63; blue = 0; break;
		case 3: red = 31; green = 63 - 2 * (value % 32); blue = 0; break;
	}
	return (red << 11) + (green << 5) + blue;
}
/// @endcond
