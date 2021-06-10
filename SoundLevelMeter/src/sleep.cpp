// InterruptPlayground.cpp
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string>
#include "ArduinoLowPower.h"
#include "sleep.h"


/*
Introduction to the course 31070 by Asger and lasse

This script emphasises the usage of interrupts
*/

int const buttonPin = A0;

volatile int sleep_flag = 0; 



void setupSleep()
{
    LowPower.attachInterruptWakeup(buttonPin, wakeupfunc, CHANGE);
    pinMode(buttonPin, INPUT);

}

void loopSleep()
{       
   buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
        LowPower.sleep();
  } 
}

void wakeupfunc()
{
    Serial.println("woken up"); 

}
