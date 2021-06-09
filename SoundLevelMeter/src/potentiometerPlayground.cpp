#include <Arduino.h>


/* 
  Sketch generated by the Arduino IoT Cloud Thing "Untitled"
  https://create.arduino.cc/cloud/things/ddb5b299-3f31-4cd1-9061-4f4462e501dc 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  String test1;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/




int const potPin = A2; 
int dBread = 0;       // variable to store the value coming from the sensor
int dBlim = 0; 


void setupPotentiometer() 
{
  
}

void loopPotentiometer() {
  dBread = analogRead(potPin);    // read the value from the sensor
 if (dBread <= 250)
 {
   dBlim = 50; 
 }
 else if (dBread <= 500)
 {
   dBlim = 60; 
 }
 else if (dBread <= 750)
 {
   dBlim = 70; 
 }
else if (dBread <= 1000)
 {
   dBlim = 80; 
 }
 else
 {
   dBlim = 90; 
 }


Serial.println(dBlim); 
}