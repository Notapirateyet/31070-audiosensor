// Downloaded libraries
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string>
// Custom libraries
//#include "InterruptPlayground.h"
#include "microphonePlayground.h"
#include "cloudPlayground.h"
//#include "potentiometerPlayground.h"
#include "lcdSoundmeter.h"
#include "LEDSoundmeter.h"
#include "sleep.h"

// Timing
bool measure_loop_time = false;
unsigned long loop_max_fs, loop_min_fs, loop_last_sample_time;

void setup()
{
  // Open the serial communications and wait for port to open:
  // Baud rate of 115200 is used instead of 9600 for a faster data rate, which means better quality
  Serial.begin(9600);
  delay(1200); // Gives a chance for the serial monitor to find something

  setupCloud();
  setupLCD();
  setupLED();
  setupMicrophone(); // After LED
  setupSleep();
}

void loop()
{
  loopMicrophone();
  loopCloud();
  //loopPotentiometer();
  loopLCD();
  //loopLED();
  loopSleep();

  if (measure_loop_time == true) // The ""== true" is for readability purposes
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
    Serial.print("Timing, loop, min: ");
    Serial.print(loop_min_fs);
    Serial.print("; max: ");
    Serial.print(loop_max_fs);
    Serial.print("; Current: ");
    Serial.println(current_fs);
  }
}
