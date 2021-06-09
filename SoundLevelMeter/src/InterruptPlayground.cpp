// InterruptPlayground.cpp
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string>
#include "ArduinoLowPower.h"


/*
Introduction to the course 31070 by Asger and lasse

This script emphasises the usage of interrupts
*/

int const LEDPIN1 = 1;
int const LEDPIN2 = 2;
int const INTERRUPTPIN = 0;


volatile int LED_on_flag = 0;
volatile int sleep_flag = 0; 

void toggleLED1()
{
    if (LED_on_flag)
    {
        digitalWrite(LEDPIN1, 0);
        LED_on_flag = 0;
    }
    else
    {
        digitalWrite(LEDPIN1, 1);
        LED_on_flag = 1;
        sleep_flag = 1; 
    }
}

void setupInterrupt()
{
    pinMode(LEDPIN1, OUTPUT);
    pinMode(LEDPIN2, OUTPUT);
    pinMode(INTERRUPTPIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), toggleLED1, CHANGE);
}

void loopInterrupt()
{
    digitalWrite(LEDPIN2, 1);
    delay(1000);
    digitalWrite(LEDPIN2, 0);
    delay(1000);
    if (sleep_flag == 1)
    {
        LowPower.sleep(5000);
    }
}
