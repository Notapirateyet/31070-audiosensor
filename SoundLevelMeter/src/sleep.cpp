// sleep.cpp
#include "sleep.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string>
#include <ArduinoLowPower.h>
#include "lcdSoundmeter.h"
#include "LEDSoundmeter.h"
#include "cloudPlayground.h"

/*
Introduction to the course 31070 by Asger and lasse

This script emphasises the usage of interrupts
*/

int const buttonPin = A1;

volatile int sleep_count = 0;
int sleep_count_old = 0;
int buttonState = 0;
bool sleep_bool = false; 

void wakeupfunc()
{
    sleep_count++;
}

// This function can be used to turn off all LED's before sleeping
void before_sleeping()
{
    writeLedCurrentValue(-1.0);
}

void setupSleep()
{
    pinMode(buttonPin, INPUT_PULLUP); // Button is connected to GND
    LowPower.attachInterruptWakeup(buttonPin, wakeupfunc, FALLING);
}

void loopSleep()
{
    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH)
    {
        lcd.setCursor(3, 1);
        lcd.write("Button up");
        lcd.print(sleep_count);
        // turn LED on:
        sleep_count_old = sleep_count;
    }
    else if (buttonState == LOW)
    {
        lcd.setCursor(3, 1);
        lcd.write(byte(0));
        lcd.write(byte(1));
        lcd.write("   down");
        if (sleep_count_old == sleep_count)
        {
            before_sleeping();
            LowPower.sleep();
        }
    }
    sleep_bool = get_sleep_val(); 

    if (sleep_bool == true)
        {
            before_sleeping();
            LowPower.deepSleep();
        }

}
