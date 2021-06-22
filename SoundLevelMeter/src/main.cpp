// Downloaded libraries
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string>
// Custom libraries

#include "microphonePlayground.h"
#include "cloudPlayground.h"
#include "lcdSoundmeter.h"
#include "LEDSoundmeter.h"
#include "sleep.h"

// Timing
bool measure_loop_time = false;
unsigned long loop_max_fs, loop_min_fs, loop_last_sample_time;

void setup()
{
  // Open the serial communications and wait for port to open:
  Serial.begin(9600);
  delay(1200); // Gives a chance for the serial monitor to find something

  setupCloud();      // Initialize the WiFi chip
  setupLCD();        // Initialize LCD
  setupLED();        // Initialize LED
  setupMicrophone(); // Set values, run after LED
  setupSleep(setup); // Prepare sleep and interrupts
  // Timing
  if (measure_loop_time)
  {
    loop_max_fs = 0;
    loop_min_fs = UINT16_MAX;
    loop_last_sample_time = micros();
  }
}

void loop()
{
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY; // Clear WTD bit, the cloud code needs this if not used
  loopMicrophone();                     // Measures and handles output
  loopCloud();                          // Synchronizes with the arduino cloud
  //loopLCD(); // Makes 3 bars, not required.
  //loopLED(); // Turns the LED's on and off
  loopSleep(); // Checks if the sleeping conditions are met

  //
  // Timing
  //
  if (measure_loop_time == true) // The "== true" is for readability purposes
  {
    // Timing measurements, measures the time since the last update of loop_last_sample_time
    unsigned long current_fs;
    current_fs = micros() - loop_last_sample_time; // Calculate sample time
    if (current_fs > loop_max_fs)
    {
      loop_max_fs = current_fs;
    }
    else if (current_fs < loop_min_fs)
    {
      loop_min_fs = current_fs;
    }
    loop_last_sample_time = micros();
    using_ISP_variable_flag = true; // Sampling is turned off to not interrupt serial communication
    //print to serial port - used for debugging
    Serial.print("Timing, loop, min: ");
    Serial.print(loop_min_fs);
    Serial.print("; max: ");
    Serial.print(loop_max_fs);
    Serial.print("; Current: ");
    Serial.println(current_fs);
    using_ISP_variable_flag = false;
  }
}
