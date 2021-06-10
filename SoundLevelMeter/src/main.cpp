// Downloaded libraries
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string>
// Custom libraries
//#include "InterruptPlayground.h"
#include "microphonePlayground.h"
#include "cloudPlayground.h"
#include "potentiometerPlayground.h"
#include "lcdSoundmeter.h"
#include "LEDSoundmeter.h"

// Create class for the LED control, using new pins and minmax values 0-8
int LEDpins[8] = {13, 12, 11, 10, 9, 8, 7, 6}; // Reverse order pins
// LedMeter earMeter(0, 8, LEDpins);
LedMeter earMeter(0, 8); // Use this for default pins

void setup()
{
  //setupMicrophone();
  //setupCloud();
  setupLCD();
  setupLED(&earMeter);
}

void loop()
{
  //loopMicrophone();
  //loopCloud();
  //loopPotentiometer();
  loopLCD();
  loopLED();
}