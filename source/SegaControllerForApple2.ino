//
// SegaControllerForApple2.ino
// https://github.com/thinghacker/SegaControllerForApple2/
//
// Use a Sega Megadrive or Genesis Controller with your Apple 2
//
// Use the Sega Start Button to Cycle between Digital and two Analog like joystick modes
//  Digital Joystick Mode auto centres when a direction is released
//  Analog Joystick Mode does not auto centre, however Button "C" will centre if required
// Buttons A and B act as Fire 0 and Fire 1 respectively
//  Hold C and Press A or B to enable/disable autofire for that fire button
//  Hold C, A and B to swap Fire 0 and Fire 1
//
// Bill of Materials to populate the PCB:
//  U1 - Ardunio Nano v3
//  U2 - Microchip MCP42100 Dual Digital 100KOhm Pot
//  U3 - Microchip MCP42100 Dual Digital 100KOhm Pot
//  R1 - 680Ohm Resistor
//  R2 - 1KOhm Resistor
//  R3 - 680Ohm Resistor
//  R4 - 1KOhm Resistor
//  Q1 - BC557 PNP Transistor
//  Q2 - BC557 PNP Transistor
//  J1 - DB9 Socket (Sega Controller Receptical)
//  J2 - IDC-10 Right Angle connector (For Apple 2 Connector)
//  J3 - 4x1 Row Connector (For I2C Connected OLED Display - I prefer to use a SH1106 128x64 1.3" White Display)
//  J4 - 4x1 Row Connector (For Analog/Digital Breakout - usually Piezo Buzzer)
//
// The Fire Button Circuit used is based on the diagram http://apple2.org.za/gswv/a2zine/faqs/R030PCA2RF.GIF which was originally posted at in the official Csa2 (comp.sys.apple2) Usenet newsgroup Apple II FAQs
//
// There is a joystick tester available as part of the ComputerInspector (MECC 1.0) program - this can be found on Asimov https://ftp.apple.asimov.net/images/disk_utils/diagnostics/ComputerInspector%20%28MECC%201.0%29.dsk
//  It is quite helpful in visually confirming what the Apple thinks the joypad is doing and to program the calibration offsets

// Apple 2 Basic Controller Test program:
//  20 PRINT "X= "; PDL(0); TAB(15); "Y= ";PDL(1); TAB(30);
//  30 IF PEEK(49249)>127 THEN PRINT "  B0";
//  40 IF PEEK(49250)>127 THEN PRINT "  B1";
//  50 PRINT: GOTO 20 
//
// Modes:
//  Normal Operation:
//   Start = Cycles between Analog 1 (Fine grained steps along the x/y axis), Analog 2 (Coarser steps along the x/y axis) and Digital Modes
//   Up/Down/Left/Right = joypad moving in relevant direction (only Digital Mode auto centres when released)
//   Button A = Fire Button 0 (Hold C and Press A to enable/disable Autofire for Fire Button 0)
//   Button B = Fire Button 0 (Hold C and Press A to enable/disable Autofire for Fire Button 1)
//   Button C (Single Press) = Centres the joypad X and Y Values (Useful in Analog Modes for rapid centering)
//   Button C (Hold and Press Start) = Enter Calibration Mode
// 
//  Calibration Mode:
//   Start = Exit Calibration Mode and save modified X/Y offsets into eeprom
//   Up/Down/Left/Right = moves the x/y centering offset in relevant direction (The Centre should appear as 128/128 on the Apple 2)
//
// Acknowlegements:
//  This program uses the Adafruit inspired SH1106 library from https://github.com/wonho-maker/Adafruit_SH1106
//  This program uses the MCP42100 code from http://www.henrykoch.de/en/arduino-control-and-test-digital-potentiometer-mcp42010-on-a-breadboard
//  This program uses Sega Controller Code From https://github.com/jonthysell/SegaController
// Copyright notice for SegaController:
//
// Author:
//       Jon Thysell <thysell@gmail.com>
//
// Copyright (c) 2017 Jon Thysell <http://jonthysell.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.



#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>
#include <EEPROM.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_ADDR    0x3C // I2C 128x64
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SH1106 display(OLED_RESET);

//#define DEBUG        1 // operating in debug mode

#define piezoPin         9 // D9 is the for the Piezo Transducer
#define slaveselectpin  10 // D10 as the slave select for the MCP42100
#define initial_button0pin      A0 // A0 is used for SW0 
#define initial_button1pin      A1 // A1 is used for SW1
#define Analog1Stepping 32 // Analog Mode 1 Pot Increment/Decrement
#define Analog2Stepping 64 // Analog Mode 2 Pot Increment/Decrement
#define HybridStepping  96 // Hybrid Pot Increment/Decrement
#define potxcentre  192 // Centre
#define potycentre  192 // Centre
#define default_potxoffset  0 // Centre Offset
#define default_potyoffset  0 // Centre Offset
#define potxminval       0 
#define potxmaxval     383 
#define potyminval       0 
#define potymaxval     383

int AnalogStepsize     = Analog1Stepping; 
int button0pin = initial_button0pin;
int button1pin = initial_button1pin;
bool fireswap = false;

#define CONFIG_START 32
typedef struct
{
    char version[6]; // Store String "SEGA" (Null Terminated)
    int potxoffset;
    int potyoffset;
}   configuration_type;

#define CONFIG_VERSION "SEGA"
// loaded with DEFAULT values - may be overwritten when loaded

configuration_type CONFIGURATION = {
    CONFIG_VERSION,
    default_potxoffset,
    default_potyoffset,
};

enum MainLoopState {
  GAMEPAD,
  CALIBRATION,
} state;


enum Gamepadmodes {
  DIGITAL,
  ANALOG1,
  ANALOG2,
  HYBRID,  
} gamepadmode;

#define CROSSHAIR_BMP_WIDTH    8
#define CROSSHAIR_BMP_HEIGHT   8
#define CROSSHAIR_DISP_MULTIPLIER    (SCREEN_HEIGHT - CROSSHAIR_BMP_HEIGHT)
static const unsigned char PROGMEM CROSSHAIR_BMP [] = {
0x1C, 0x08, 0x5D, 0x77, 0x5D, 0x08, 0x1C, 0x00, 
};

#define LOGO_BMP_WIDTH    120
#define LOGO_BMP_HEIGHT   64
static const unsigned char PROGMEM LOGO_BMP [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0xE0, 0x7F, 0xF8, 0x1F, 0xFF, 0x80, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xE1,
0xFF, 0xF8, 0x7F, 0xFF, 0x80, 0x3F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xE3, 0xFF,
0xF8, 0xFF, 0xFF, 0x80, 0x7F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x07, 0xC0, 0x01,
0xF0, 0x00, 0x00, 0x71, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x3F, 0xE7, 0x9F, 0xF9, 0xE7,
0xFF, 0x80, 0xF5, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0xFF, 0xE7, 0x3F, 0xF9, 0xCF, 0xFF,
0x80, 0xE4, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xFF, 0xE7, 0x7F, 0xF9, 0xDF, 0xFF, 0x80,
0xEE, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0xC0, 0x0F, 0x70, 0x03, 0xDC, 0x00, 0x01, 0xCE,
0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0xF0, 0x0F, 0x7F, 0xE3, 0xDD, 0xFF, 0x81, 0xDF, 0x70,
0x00, 0x00, 0x00, 0x00, 0x00, 0x39, 0xFF, 0x0F, 0x7F, 0xE3, 0xDD, 0xFF, 0x81, 0xDF, 0x70, 0x00,
0x00, 0x00, 0x00, 0x00, 0x1C, 0x7F, 0x0F, 0x7F, 0xE3, 0xDD, 0xFF, 0x83, 0x9F, 0x38, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1F, 0x1F, 0x8F, 0x00, 0x03, 0xDC, 0x03, 0x83, 0xBB, 0xB8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x0F, 0xC3, 0xCF, 0x7F, 0xE3, 0xDD, 0xFB, 0x87, 0xBB, 0x98, 0x00, 0x00, 0x00, 0x00,
0x00, 0x07, 0xF9, 0xEF, 0x7F, 0xE3, 0xDD, 0xFB, 0x87, 0x79, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0xFC, 0xEF, 0x7F, 0xE3, 0xDD, 0xFB, 0x87, 0x71, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x3E, 0xEF, 0x70, 0x03, 0xDC, 0x7B, 0x8E, 0x71, 0xCE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E,
0xE7, 0x7F, 0xF9, 0xDF, 0xFB, 0x8E, 0xE0, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0xE7,
0x7F, 0xF9, 0xDF, 0xFB, 0x9E, 0xFF, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF9, 0xE7, 0x3F,
0xF9, 0xCF, 0xFB, 0x9C, 0xFF, 0xE7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF3, 0xC7, 0x9F, 0xF9,
0xE7, 0xFB, 0x9D, 0xFF, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC3, 0xC0, 0x00, 0xF0,
0x03, 0xBD, 0xC0, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0x83, 0xFF, 0xF8, 0xFF, 0xFF,
0xB9, 0x9F, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFE, 0x01, 0xFF, 0xF8, 0x7F, 0xFF, 0xBB,
0xBF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF8, 0x00, 0x7F, 0xF8, 0x1F, 0xFF, 0xF3, 0xBF,
0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x7E, 0xFC, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x3C, 0x78, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x70, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xF0, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8,
0x05, 0xE0, 0x2F, 0x03, 0x07, 0xC0, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8, 0x0F,
0xF0, 0x7F, 0x83, 0x0C, 0x60, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x38, 0x1F, 0xF8,
0xFF, 0xC3, 0x18, 0x30, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x18, 0x3C, 0x39, 0xE1,
0xE3, 0x30, 0x30, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1C, 0x18, 0x0C, 0xC0, 0x63,
0x30, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1C, 0x18, 0x04, 0xC0, 0x23, 0x7F,
0xF8, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1C, 0x18, 0x06, 0xC0, 0x33, 0x70, 0x00,
0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x18, 0x06, 0xC0, 0x33, 0x70, 0x00, 0x18,
0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0C, 0x18, 0x06, 0xC0, 0x33, 0x70, 0x00, 0x18, 0x30,
0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0E, 0x18, 0x06, 0xC0, 0x33, 0x70, 0x00, 0x18, 0x30, 0x00,
0x00, 0x00, 0x00, 0x00, 0x04, 0x0E, 0x18, 0x06, 0xC0, 0x33, 0x30, 0x00, 0x18, 0x30, 0x00, 0x00,
0x00, 0x00, 0x00, 0x08, 0x06, 0x18, 0x04, 0xC0, 0x23, 0x38, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00,
0x00, 0x00, 0x08, 0x07, 0x18, 0x0C, 0xC0, 0x63, 0x38, 0x00, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00,
0x00, 0x08, 0x07, 0x18, 0x18, 0xC0, 0xC3, 0x1C, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1C, 0x0F, 0x9C, 0x30, 0xE1, 0x87, 0x9E, 0x30, 0x3C, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E,
0x1F, 0xDB, 0xE0, 0xDF, 0x0F, 0xC7, 0xC0, 0x7E, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x18, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0xC0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0xC0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x01, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


#include <SegaController.h>

//                  DB9 Pin 7,  1,  2,  3,  4,  6,  9
SegaController controller  (7,  2,  3,  4 , 5 , 6 , 8 );

// Sega Controller states
word currentState = 0;
word lastState = 0;
bool oldjoypad_startstate = LOW;
bool oldjoypad_leftstate = LOW;
bool oldjoypad_rightstate = LOW;
bool oldjoypad_upstate = LOW;
bool oldjoypad_downstate = LOW;


// Apple 2 Gameport states
int potx = potxcentre;
int oldpotx = potx;
int poty = potycentre;
int oldpoty = poty;
bool sw0 = HIGH;
bool oldsw0 = sw0;
bool sw1 = HIGH;
bool oldsw1 = sw1;
bool gamepadconnect = LOW;
bool oldgamepadconnect = HIGH;
bool sw0autofire = false;
bool oldsw0autofire = LOW;
bool sw1autofire = false;
bool oldsw1autofire = LOW;
bool oldc_start = LOW;
bool oldc_fireswap = LOW;


void oleddisplay(word currentState) {
  char buf[40];
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

#ifdef DEBUG
  display.setCursor(0,0);
  display.print((currentState & SC_CTL_ON)    ? "+" : "-");
  display.print((currentState & SC_BTN_UP)    ? "U" : "0");
  display.print((currentState & SC_BTN_DOWN)  ? "D" : "0");
  display.print((currentState & SC_BTN_LEFT)  ? "L" : "0");
  display.print((currentState & SC_BTN_RIGHT) ? "R" : "0");
  display.print((currentState & SC_BTN_START) ? "S" : "0");
  display.print((currentState & SC_BTN_A)     ? "A" : "0");
  display.print((currentState & SC_BTN_B)     ? "B" : "0");
  display.print((currentState & SC_BTN_C)     ? "C" : "0");
  display.print((currentState & SC_BTN_X)     ? "X" : "0");
  display.print((currentState & SC_BTN_Y)     ? "Y" : "0");
  display.print((currentState & SC_BTN_Z)     ? "Z" : "0");
  display.print((currentState & SC_BTN_MODE)  ? "M" : "0");

  display.setCursor(0,16);
  //sprintf(buf, "X: %03d Y: %03d", potx, poty);
  display.print ("X: ");
  display.print (potx);
  display.print (" Y: ");
  display.println (poty);
  display.setCursor(0,24);
  display.print ("XO: ");
  display.print (CONFIGURATION.potxoffset);
  display.print (" YO: ");
  display.println (CONFIGURATION.potyoffset);
  display.setCursor(0,32);
  display.print("SW0: ");
  display.print((sw0) ? "+" : "-");
  display.print((sw0autofire) ? "A" : " ");
  display.print(" SW1: ");
  display.print((sw1) ? "+" : "-");
  display.print((sw1autofire) ? "A" : " ");

  display.setCursor(0,40);
  display.print("State: ");
  switch(state){
    case(GAMEPAD):
      display.println("Gamepad");
      break;
    case(CALIBRATION):
      display.println("Calibration");
      break;
  }
  display.setCursor(0,48);
  display.print("Mode ");
  switch(gamepadmode){
    case(DIGITAL):
      sprintf(buf, "Digital");
      break;
    case(ANALOG1):
      sprintf(buf, "Analog 1");
      break;
    case(ANALOG2):
      sprintf(buf, "Analog 2");
      break;       
    case(HYBRID):
      sprintf(buf, "Hybrid");
      break;       
  }
  display.println(buf);
  
  display.display();   
#else
  // Normal Running Code
  if (!gamepadconnect){
    display.setCursor(0,SCREEN_HEIGHT/2-8);
    display.setCursor((SCREEN_WIDTH - 5 * (strlen("SEGA Gamepad is not")+1))/2 , SCREEN_HEIGHT/2 - 8);
    display.println("SEGA Gamepad is not");  
    display.setCursor((SCREEN_WIDTH - 5 * (strlen("connected")+1))/2 , SCREEN_HEIGHT/2);
    display.println("connected");  
    display.display();   
    }
  else{
    display.drawRect(SCREEN_WIDTH - SCREEN_HEIGHT, 0, SCREEN_HEIGHT, SCREEN_HEIGHT , WHITE);
    display.setCursor(12,12);
    display.print("SW0: ");
    display.print((sw0) ? "+" : "-");
    display.print((sw0autofire) ? "A" : " ");
    display.setCursor(12,24);
    display.print("SW1: ");
    display.print((sw1) ? "+" : "-");
    display.print((sw1autofire) ? "A" : " ");
    if (fireswap){
      display.setCursor(4,32);
      display.print("ButtonSwap");      
    }
    display.setCursor(18,48);
    display.print("Mode");
    switch(state){
      case(CALIBRATION):
        display.setCursor(4,56);
        display.println("Calibrate");
        display.setCursor(72,12);
        display.print("X Offset");
        display.setCursor(85,22);
        display.print(CONFIGURATION.potxoffset);
        display.setCursor(72,32);
        display.print("Y Offset");
        display.setCursor(85,42);
        display.print(CONFIGURATION.potyoffset);
        break;
      case(GAMEPAD):
        display.drawBitmap(SCREEN_WIDTH - SCREEN_HEIGHT + (CROSSHAIR_DISP_MULTIPLIER * potx) / 384 , (CROSSHAIR_DISP_MULTIPLIER * poty) / 384,
                           CROSSHAIR_BMP, CROSSHAIR_BMP_WIDTH, CROSSHAIR_BMP_HEIGHT, WHITE);       
        display.setCursor(8,56);
        switch(gamepadmode){
          case(DIGITAL):
            display.print("Digital");
            break;
          case(ANALOG1):
            display.print("Analog 1");
            break;
          case(ANALOG2):
            display.print("Analog 2");
            break;       
          case(HYBRID):
            display.print("Hybrid");
            break;       
        } 
        break;
    }
    display.display();        
  }
  if (oldgamepadconnect != gamepadconnect){
    oldgamepadconnect = gamepadconnect;
    tone(piezoPin, 440, 20);
  }
#endif
}

void DualMSP42010PotXWrite (int xvalue) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveselectpin,LOW);
  if (xvalue < 0){
    xvalue = 0;
  }
  int x1, x2;

  if (xvalue > 255){
    x1 = 255;
    x2 = xvalue-255;
  }
  else {
    x1 = xvalue;
    x2 = 0;      
  }

  //  set the value of MCP42100 #1 Pot 0 (x)
  SPI.transfer(B00010001);
  SPI.transfer(x1);
  //  set the value of MCP42100 #0 Pot 0 (x)
  SPI.transfer(B00010001);
  SPI.transfer(x2);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveselectpin,HIGH);
}


void DualMSP42010PotYWrite (int yvalue) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveselectpin,LOW);
  if (yvalue < 0){
    yvalue = 0;
  }
  int y1, y2;

  if (yvalue > 255){
    y1 = 255;
    y2 = yvalue-255;
  }
  else {
    y1 = yvalue;
    y2 = 0;      
  }

  //  set the value of MCP42100 #1 Pot 1 (y)
  SPI.transfer(B00010010);
  SPI.transfer(y1);
  //  set the value of MCP42100 #0 Pot 1 (y)
  SPI.transfer(B00010010);
  SPI.transfer(y2);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveselectpin,HIGH);
}

int loadConfig() {
    // Look for "SEGA" to see if we have a config already present
    if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
        EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
        EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2] &&
        EEPROM.read(CONFIG_START + 3) == CONFIG_VERSION[3]) {
        // load (overwrite) the local configuration struct
        for (unsigned int i = 0; i < sizeof(CONFIGURATION); i++) { * ((char * ) & CONFIGURATION + i) = EEPROM.read(CONFIG_START + i);
        }
        return 1; // return 1 if config loaded 
    }
    return 0; // return 0 if config NOT loaded
}

// save the CONFIGURATION in to EEPROM
void saveConfig() {
    for (unsigned int i = 0; i < sizeof(CONFIGURATION); i++)
        EEPROM.write(CONFIG_START + i, * ((char * ) & CONFIGURATION + i));
}

void setup() {
  // set the buttons as an output:
  pinMode (button0pin, OUTPUT);
  digitalWrite (button0pin, !sw0);
  pinMode (button1pin, OUTPUT);
  digitalWrite (button1pin, !sw1);
  // set the slaveselectpin as an output:
  pinMode (slaveselectpin, OUTPUT);
 
  loadConfig(); // obtain the offsets
  potx = potxcentre;
  poty = potycentre;

  // initialize SPI:
  SPI.begin();
  DualMSP42010PotXWrite(potx + CONFIGURATION.potxoffset);
  DualMSP42010PotYWrite(poty + CONFIGURATION.potyoffset);

  pinMode(piezoPin,OUTPUT);
  display.begin(SH1106_SWITCHCAPVCC, OLED_ADDR);  // initialize with the I2C addr 0x3D (for the 128x64)    
  display.clearDisplay();
  display.display();
  display.drawBitmap(
    (SCREEN_WIDTH  - LOGO_BMP_WIDTH ) / 2,
    0, LOGO_BMP, LOGO_BMP_WIDTH, LOGO_BMP_HEIGHT, 1);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(6,35);
  display.print("Gamepad");
  display.setCursor(6,44);
  display.print("for the");

  // update display with all of the above graphics
  display.display();
  delay(6000);

}

void gamepad() {
  // Start
  if (currentState & SC_BTN_START && !oldjoypad_startstate)
  {
    oldjoypad_startstate = true;
    switch(gamepadmode) {
      case(DIGITAL):
        gamepadmode = ANALOG1;
        AnalogStepsize = Analog1Stepping;
        tone(piezoPin, 3300, 20);
        delay(100);
        tone(piezoPin, 3300, 20);
        break;
      case(ANALOG1):
        gamepadmode = ANALOG2;
        AnalogStepsize = Analog2Stepping;
        tone(piezoPin, 3300, 20);
        delay(100);
        tone(piezoPin, 3300, 20);
        delay(100);
        tone(piezoPin, 3300, 20);
        break;
      case(ANALOG2):
        gamepadmode = HYBRID;
        AnalogStepsize = HybridStepping;
        tone(piezoPin, 3300, 20);
        delay(100);
        tone(piezoPin, 3300, 20);
        delay(100);
        tone(piezoPin, 3300, 20);
        delay(100);
        tone(piezoPin, 3300, 20);
        break;
      case(HYBRID):
        gamepadmode = DIGITAL;
        tone(piezoPin, 3300, 20);
        break;
    }
    potx = potxcentre;
    poty = potycentre;
    oldpotx = potx;
    oldpoty = poty;
  }
  if (!(currentState & SC_BTN_START) && oldjoypad_startstate)
  {
    oldjoypad_startstate = false;    
  }

  // LEFT
  if ((currentState & SC_BTN_LEFT && !oldjoypad_leftstate))
  {
    oldjoypad_leftstate = true;
    switch(gamepadmode) {
      case(DIGITAL):
        potx = potxminval;
        break;
      case(HYBRID):
        if ( potx > potxcentre) {
          potx = potxcentre;
          break;
        }
      case(ANALOG1):
      case(ANALOG2):
        if ( (potx - AnalogStepsize) > potxminval){
          potx = potx - AnalogStepsize;
        }
        else{
          potx = potxminval; 
        }
        break;
    }
  }
  if (!(currentState & SC_BTN_LEFT) && oldjoypad_leftstate)
  {
    oldjoypad_leftstate = false;    
    if (gamepadmode == DIGITAL) {
      potx = potxcentre;
    }
  }

  // RIGHT
  if ((currentState & SC_BTN_RIGHT && !oldjoypad_rightstate))
  {
    oldjoypad_rightstate = true;
    switch(gamepadmode) {
      case(DIGITAL):
        potx = potxmaxval;
        break;
      case(HYBRID):
        if ( potx < potxcentre) {
          potx = potxcentre;
          break;
        }
      case(ANALOG1):
      case(ANALOG2):
        if ( (potx + AnalogStepsize) < potxmaxval){
          potx = potx + AnalogStepsize;
        }
        else{
          potx = potxmaxval; 
        }
        break;
    }
  }
  if (!(currentState & SC_BTN_RIGHT) && oldjoypad_rightstate)
  {
    oldjoypad_rightstate = false;    
    if (gamepadmode == DIGITAL) {
      potx = potxcentre;
    }
  }
  
  // UP
  if ((currentState & SC_BTN_UP && !oldjoypad_upstate))
  {
    oldjoypad_upstate = true;
    switch(gamepadmode) {
      case(DIGITAL):
        poty = potyminval;
        break;
      case(HYBRID):
        if ( poty > potycentre) {
          poty = potycentre;
          break;
        }
      case(ANALOG1):
      case(ANALOG2):
        if ( (poty - AnalogStepsize) > potyminval){
          poty = poty - AnalogStepsize;
        }
        else{
          poty = potyminval; 
        }
        break;
    }
  }
  if (!(currentState & SC_BTN_UP) && oldjoypad_upstate)
  {
    oldjoypad_upstate = false;    
    if (gamepadmode == DIGITAL) {
      poty = potycentre;
    }
  }

  // DOWN
  if ((currentState & SC_BTN_DOWN && !oldjoypad_downstate))
  {
    oldjoypad_downstate = true;
    switch(gamepadmode) {
      case(DIGITAL):
        poty = potymaxval;
        break;
      case(HYBRID):
        if ( poty < potycentre) {
          poty = potycentre;
          break;
        }
      case(ANALOG1):
      case(ANALOG2):
        if ( (poty + AnalogStepsize) < potymaxval){
          poty = poty + AnalogStepsize;
        }
        else{
          poty = potymaxval; 
        }
        break;
    }
  }
  if (!(currentState & SC_BTN_DOWN) && oldjoypad_downstate)
  {
    oldjoypad_downstate = false;    
    if (gamepadmode == DIGITAL) {
      poty = potycentre;
    }
  }

  // Button C Firebutton Swap, Autofire Toggle, or Enter Calibration Mode
  if (currentState & SC_BTN_C)
  {
    // Firebutton Swap
    if ((currentState & SC_BTN_A) && (currentState & SC_BTN_B) && !oldc_fireswap){
      fireswap = !fireswap;
      if (fireswap){
        button0pin = initial_button1pin;
        button1pin = initial_button0pin;        
      }
      else {
        button0pin = initial_button0pin;
        button1pin = initial_button1pin;
      }
      oldc_fireswap = true;
    }
    // Toggle autofire SW0
    if ((currentState & SC_BTN_A) && !(currentState & SC_BTN_B) && !oldsw0autofire){
      oldsw0autofire = true;
      sw0autofire = !sw0autofire;
    }
    // Toggle autofire SW1
    else if ((currentState & SC_BTN_B) && !(currentState & SC_BTN_A) && !oldsw1autofire){
      oldsw1autofire = true;
      sw1autofire = !sw1autofire;     
    }
    // Enable Calibration Mode
    else if ((currentState & SC_BTN_START) && !oldc_start){
      oldc_start = true;
      state = CALIBRATION;
    }    
    // Pot to Centre
    else{
      potx = potxcentre;
      poty = potycentre;
      tone(piezoPin, 1300, 20);
    }
  }
  if (!(currentState & SC_BTN_C))
  {
    oldsw0autofire = false;
    oldsw1autofire = false;
    oldc_start = false;
    oldc_fireswap = false;
  }
  
  // Fire Button 1 (Switch 0) 
  if (sw0autofire && (currentState & SC_BTN_A))
  {
    sw0 = !sw0;
  }
  else {
    sw0 = (currentState & SC_BTN_A) ? HIGH : LOW;      
  }
  // Fire Button 2 (Switch 1) 
  if (sw1autofire && (currentState & SC_BTN_B))
  {
    sw1 = !sw1;
  }
  else {
    sw1 = (currentState & SC_BTN_B) ? HIGH : LOW;      
  }
}

void potcalibration(){
  // set oldc_start to false when the start button has been let go
  if (!(currentState & SC_BTN_START) && oldc_start)
  {
    oldc_start = false;
    tone(piezoPin, 2600, 20);
  }
  if (!oldc_start)
  {
    // change the pots if a direction button is being held
    if ((currentState & SC_BTN_LEFT && !oldjoypad_leftstate))
    {
      oldjoypad_leftstate = true;
      if (--CONFIGURATION.potxoffset < -127){
          CONFIGURATION.potxoffset = -127;
      }
    }
    if (!(currentState & SC_BTN_LEFT) && oldjoypad_leftstate)
    {
      oldjoypad_leftstate = false;    
    }
    if ((currentState & SC_BTN_RIGHT && !oldjoypad_rightstate))
    {
      oldjoypad_rightstate = true;
      if (++CONFIGURATION.potxoffset > 127){
          CONFIGURATION.potxoffset = 127;
      }
    }
    if (!(currentState & SC_BTN_RIGHT) && oldjoypad_rightstate)
    {
      oldjoypad_rightstate = false;    
    }
    if ((currentState & SC_BTN_UP && !oldjoypad_upstate))
    {
      oldjoypad_upstate = true;
      if (--CONFIGURATION.potyoffset < -127){
          CONFIGURATION.potyoffset = -127;
      }
    }
    if (!(currentState & SC_BTN_UP) && oldjoypad_upstate)
    {
      oldjoypad_upstate = false;    
    }
    if ((currentState & SC_BTN_DOWN && !oldjoypad_downstate))
    {
      oldjoypad_downstate = true;
      if (++CONFIGURATION.potyoffset > 127){
          CONFIGURATION.potyoffset = 127;
      }
    }
    if (!(currentState & SC_BTN_DOWN) && oldjoypad_downstate)
    {
      oldjoypad_downstate = false;    
    }
    // go back to gamepad mode when the start button is pressed and oldc_start is false
    if ((currentState & SC_BTN_START) && !oldc_start)
    {
      tone(piezoPin, 2600, 20);
      saveConfig();
      delay(100);
      tone(piezoPin, 2600, 20);
      state = GAMEPAD;
      potx = potxcentre;
      poty = potycentre; 
    } 
  }
}

void loop() {
  currentState = controller.getState();
  if (currentState != lastState)
  {
    lastState = currentState;
  }
  gamepadconnect = (currentState & SC_CTL_ON) ? HIGH : LOW;
  switch(state){
    case(GAMEPAD):
      gamepad();
      break;
    case(CALIBRATION):
      potcalibration();
      break;
  }
  if (currentState != lastState)
  {
    lastState = currentState;
  }
  // Update pot values
  DualMSP42010PotXWrite(potx + CONFIGURATION.potxoffset);
  DualMSP42010PotYWrite(poty + CONFIGURATION.potyoffset);
  // Update Fire Buttons
  if (sw0 != oldsw0)
  {
    oldsw0 = sw0;
    digitalWrite(button0pin, !sw0);
  }
  if (sw1 != oldsw1)
  {
    oldsw1 = sw1;
    digitalWrite(button1pin, !sw1);
  }  
  oleddisplay(currentState);
}
