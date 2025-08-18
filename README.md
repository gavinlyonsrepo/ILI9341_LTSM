# ILI9341 LCD TFT Readme

[![Image TFT](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341.jpg)

## Table of contents

* [Overview](#overview)
* [Installation](#installation)
* [Dependency](#dependency)
* [Documentation](#documentation)
* [Software](#software)
	* [Examples](#examples)
	* [SPI](#spi)
* [Hardware](#hardware)
* [Tested](#tested)
* [Output](#output)
* [Notes and Issues](#notes-and-issues)


## Overview

* Name: ILI9341_LTSM
* Description:

C++ Library for the ILI9341 TFT SPI LCD for the Arduino Eco-system.

1. Arduino eco-system library.
2. Invert, Scroll, rotate and sleep control.
3. 16 ASCII fonts included, fonts can easily be removed or added.
4. Graphics class included.
5. Frame buffer mode included.
6. Bitmaps supported: Sprites + 1, 8 and 16 bit color bitmaps
7. Hardware & software SPI options
8. Read Diagnostics function.
9. [Project url link](https://github.com/gavinlyonsrepo/ILI9341_LTSM)

## Installation

The library is included in the official Arduino library manger and the optimum way to install it is using the library manager which can be opened by the *manage libraries* option in Arduino IDE.

## Dependency

This library requires the Arduino library 'display16_LTSM' as a dependency. display16_LTSM library contains
the graphics, bitmaps, and font methods as well as font data and bitmap test data. Its also 
where the user sets options(debug, advanced graphics and frame buffer mode).
When you install 'ILI9341_LTSM' with Arduino IDE. It should install 'display16_LTSM' as well after 
a prompt, if it does not you have to install it same way as 'ILI9341_LTSM'.
The 'display16_LTSM' project and readme is at [URL github link.](https://github.com/gavinlyonsrepo/display16_LTSM)
'display16_LTSM' is also written by author of this library. 

## Documentation

Code is commented for the 'doxygen' API generation tool.
Documents on fonts, bitmaps and graphics can be found at 
the dependency 'display16_LTSM' repository, [URL github link](https://github.com/gavinlyonsrepo/display16_LTSM)

## Software

There are 6 example files included.

| Filename .ino | Function  | Note |
| --- | --- | --- |
| HELLO WORLD | Hello world basic use case | --- |
| MISC| Text, Graphics, Functions| dislib16 ADVANCED GRAPHICS ENABLE must be enabled for all tests to work |
| READ DIAGS| Read display diagnostics | Requires an extra GPIO pin: Miso |
| BITMAP| 1,8 & 16 bit bitmaps tests + bitmap FPS tests| Bitmap test data is stored in arrays |
| DEMO| A demo showing the unit circle plotted on graph | ---- |
| FRAME BUFFER | Testing frame Buffer mode | dislib16 ADVANCED SCREEN BUFFER ENABLE must be enabled  |

### SPI

In the example files, there are 2 sections in "setup()" function 
where user can make adjustments. 

 * User Option 1 Setup()       Setup the GPIO & SPI
 * User Option 2 Setup()       Screen Setup 

*USER OPTION 1 GPIO SPI SPEED*

Two different constructors which one is called depends on 'bhardwareSPI', 
true for hardware spi, false for software SPI.

Hardware SPI:

Here the user can pass the SPI Bus freq in Hertz, Currently set to 8 Mhz,
and the Reset, chip select and data or command line. Any GPIO can be used for these.
The MOSI and CLk are tied to default MCU SPI interface GPIO.

Software SPI:

The optional GPIO software uS delay,which by default is zero.
Setting this higher can be used to slow down Software SPI
which may be beneficial on Fast MCU's.
The 5 GPIO pins used. Any GPIO can be used for these.

*USER OPTION 2 Screen size*

User can adjust screen pixel height, screen pixel width.

## Hardware

Tested and developed on:

* Size 2.4 inch SPI Serial  IPS color TFT LCD
* Resolution: 240 (H) RGB x 320 (V)
* Color Depth: 262K/65K (65K used)
* Control chip:ILI9341
* Display area 27.972 (H) x 32.634 (V)
* Logic voltage 3.3V
* Touch panel with XPT2046 IC

Connections as setup in HELLOWORLD.ino example  file.

| PinNum | Pin description | HW SPI  | SW SPI |note |
| --- | --- | --- | --- | --- |
| 1 | VCC | VCC |VCC| 3.3 or 5V ,CAUTION your display must have 3.3V regulator on back to connect to 5V |
| 2 | GND | GND |GND  | |
| 3 | CS | 15 |15 | Chip select, Use any GPIO for this line|
| 4 | RESET | 4 |4 | If no reset pin, pass -1 in here & display will use software rst|
| 5 | DC | 5 |5 | Data or command, Use any GPIO for this line |
| 6 | SDI(MOSI) | SPI MOSI | 13 | |
| 7 | SCLK | SPI CLK | 12 |  |
| 8 | LED | VCC |VCC |CAUTION Your display may need current limit resistor|
| 9 | SDO(MISO) | SPI MISO | 14 |Only needed to read diagnostics from TFT see READ DIAG example. pass -1 to not use(default) |
| 10| T_CLK | n/a  | n/a| needed for xpt2046 touchscreen(not implemented yet) |
| 11| T_CS | n/a | n/a |needed for xpt2046 touchscreen(not implemented yet) |
| 12| T_DIN | n/a | n/a | needed for xpt2046 touchscreen(not implemented yet)|
| 13 | T_DO | n/a | n/a | needed for xpt2046 touchscreen(not implemented yet)|
| 14 | T_IRQ | n/a | n/a |needed  for xpt2046 touchscreen(not implemented yet) |


1. This is a 3.3V logic device do NOT connect the I/O logic lines to 5V logic device.
2. LED Backlight control is left to user.
3. Pins marked with T_ prefix are related to the touchscreen IC XP2046 if user is not using the touch screen do not connect these. (not implemented yet)

## Output

Four-Byte Burger 240x320 16-bit bitmap test image, Credits [Ahoy](https://www.youtube.com/watch?v=i4EFkspO5p4)

[![output pic](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output2.jpg)](https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/extra/images/ili9341output2.jpg)

