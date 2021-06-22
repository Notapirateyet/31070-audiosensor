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
unsigned long cloud_update_rate = 5000; // [ms] updates every 5 seconds to the cloud

// Timing
bool measure_cloud_time = false;
unsigned long cloud_average_fs, cloud_max_fs, cloud_min_fs, cloud_last_sample_time; //Time the measurements with the cloud

void setupCloud()
{
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
  last_cloud_update = millis(); //timer for last cloud update

  if (measure_cloud_time)
  {
    cloud_average_fs = 0; //sampling time for cloud
    cloud_max_fs = 0;
    cloud_min_fs = INT16_MAX;
    cloud_last_sample_time = 0;
  }
}

void loopCloud()
{
  current_cloud_time = millis();
  if (current_cloud_time - last_cloud_update >= cloud_update_rate)
  {

    if (measure_cloud_time == true)
    {
      cloud_last_sample_time = micros(); // Update timing variable
    }

    last_cloud_update = current_cloud_time;
    ArduinoCloud.update(); //update the variables to the cloud

    if (measure_cloud_time == true)
    {
      // Timing measurements, measures the time since the last update of cloud_sample_time
      unsigned long current_fs;
      current_fs = micros() - cloud_last_sample_time;         // Calculate sample time
      cloud_average_fs = (cloud_average_fs + current_fs) / 2; // Not really an average
      if (current_fs > cloud_max_fs)
      {
        cloud_max_fs = current_fs;
      }
      else if (current_fs < cloud_min_fs)
      {
        cloud_min_fs = current_fs;
      }
      Serial.print("Timing, min: ");
      Serial.print(cloud_min_fs);
      Serial.print("; max: ");
      Serial.print(cloud_max_fs);
      Serial.print("; Average: ");
      Serial.println(cloud_average_fs); //Print the timing from the cloud 
    }
  }
}

void onTest1Change()
{
  // Serial.println(test1); // Print the string to serial on a change - used for testing 
}

void onTest2Change()
{
  // Do something - test function not used
}

void onSleepValChange()
{
  // Do something - test function not used
}

void onDBBoundaryChange()
{
  earMeter.set_max_value_dB(dB_boundary); //pass the dB boundary to the earmeter class
}

void write_dB_read(int new_dB_read)
{
  dB_read = new_dB_read; //set dB read value locally from IoT cloud
}

void write_dB_boundary(int new_dB_boundary)
{
  dB_boundary = new_dB_boundary; //set local dB boundary from IoT cloud
}

bool get_sleep_val()
{
  return sleep_Val; //get sleep value from IoT cloud
}

void write_sleep_val(bool new_sleep_val)
{
  sleep_Val = new_sleep_val; //set local sleep value to true or false from IoT cloud
}