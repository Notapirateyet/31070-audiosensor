// sleep.cpp
#include "sleep.h"
#include <LiquidCrystal.h>
#include <string>
#include <ArduinoLowPower.h>
#include "lcdSoundmeter.h"
#include "LEDSoundmeter.h"
#include "cloudPlayground.h"
#include <SPI.h>
#include <WiFi101.h>

/*
Introduction to the course 31070 by Asger and lasse

This script emphasises the usage of interrupts
*/

int const buttonPin = A1;

volatile int sleep_count = 0;
int sleep_count_old = 0;
int buttonState = 0;
volatile bool sleep_bool = false;
volatile bool wake_up_flag = false;
bool first_time_pressed = false;
voidFuncPtr main_setup_ptr;

void wakeupfunc_ISR()
{
    // Count button presses
    sleep_count++;
    // Only do the following when waking up
    if (sleep_bool == true)
    {
        sleep_bool = false;
        wake_up_flag = true;
        WDT->CTRL.bit.ENABLE = 1; // Turn watchdog back on
    }
}

// This function can be used to turn off all LED's before sleeping
void before_sleeping()
{
    writeLedCurrentValue(-1.0); // Turn off LED's
    WiFi.end();
    lcd.clear();
    Serial.println("Sleeping zzZZZzzz");
    Serial.end();
    // delay(500);
    WDT->CTRL.bit.ENABLE = 0; // Turn watchdog off
}

void setupSleep(voidFuncPtr main_setup)
{
    pinMode(buttonPin, INPUT_PULLUP); // Button is connected to GND
    LowPower.attachInterruptWakeup(buttonPin, wakeupfunc_ISR, RISING);
}

void loopSleep()
{
    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is LOW:
    if (buttonState == HIGH)
    {
        lcd.setCursor(12, 1);
        lcd.write("up:");
        lcd.print(sleep_count);
        // turn LED on:
        sleep_count_old = sleep_count;

        if (sleep_bool == true)
        {
            before_sleeping();
            LowPower.sleep();
        }
        first_time_pressed = true; // reset the button press value used below.
    }
    else if (buttonState == LOW)
    {
        lcd.setCursor(10, 1);
        lcd.write("down:");
        if (first_time_pressed)
        {
            sleep_bool = true;
            before_sleeping();
            LowPower.sleep();
        }
        first_time_pressed = false;
    }

    // Do the following when exiting the sleep mode
    if (wake_up_flag)
    {
        write_sleep_val(false);
        main_setup_ptr();
        wake_up_flag = false;
    }
    sleep_bool = get_sleep_val();
}
