// microphonePlayground.cpp

#include <Arduino.h>
#include <I2S.h>

//Parameters
const int micPin  = A0;

//Variables
int micVal  = 0;

void setupMicrophone()
{

    // Open the serial communications and wait for port to open:
    // Baud rate of 115200 is used instead of 9600 for a faster data rate, which means better quality

    Serial.begin(115200);
    pinMode(micPin, INPUT);

}

void loopMicrophone()
{
  analogReadResolution(16); //resolution is set as high as possible
  micVal = analogRead(micPin); //the analog value is read from the microphone
}