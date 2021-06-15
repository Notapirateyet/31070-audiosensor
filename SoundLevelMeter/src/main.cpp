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
#include "sleep.h"

void setup()
{
  // Open the serial communications and wait for port to open:
  // Baud rate of 115200 is used instead of 9600 for a faster data rate, which means better quality
  Serial.begin(9600);
  setupMicrophone();
  //setupCloud();
  setupLCD();
  setupLED();
  setupSleep();
}

void loop()
{
  loopMicrophone();
  //loopCloud();
  //loopPotentiometer();
  loopLCD();
  loopLED();
  loopSleep();
}