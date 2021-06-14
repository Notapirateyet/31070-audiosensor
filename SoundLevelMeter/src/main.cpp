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
  //setupMicrophone();
  //setupCloud();
  setupLCD();
  setupLED();
  setupSleep();
}

void loop()
{
  //loopMicrophone();
  //loopCloud();
  //loopPotentiometer();
  loopLCD();
  loopLED();
  loopSleep();
}