#include <Arduino.h>
#include "arduino_secrets.h"
#include "thingProperties.h"
#include "LEDSoundmeter.h"

/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/ddb5b299-3f31-4cd1-9061-4f4462e501dc 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  int dB_boundary;
  int dB_read;
  String test1;
  CloudAcceleration test2;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/
// Definitions
unsigned long last_cloud_update, current_cloud_time;
unsigned long cloud_update_rate = 5000; // [ms]

void setupCloud()
{
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  last_cloud_update = millis();
}

void loopCloud()
{
  current_cloud_time = millis();
  if (current_cloud_time - last_cloud_update >= cloud_update_rate)
  {
    last_cloud_update = current_cloud_time;
    ArduinoCloud.update();
  }
}

void onTest1Change()
{
  // Serial.println(test1); // Print the string to serial on a change
}

void onTest2Change()
{
  // Do something
}

void onSleepValChange() {
  // Do something
}


void onDBBoundaryChange()
{
  earMeter.set_max_value(dB_boundary);
}

void write_dB_read(int new_dB_read)
{
  dB_read = new_dB_read;
}

void write_dB_boundary(int new_dB_boundary)
{
  dB_boundary = new_dB_boundary;
}

bool get_sleep_val()
{
  return sleep_Val; 
}

bool write_sleep_val(bool new_sleep_val)
{
  sleep_Val = new_sleep_val; 
}