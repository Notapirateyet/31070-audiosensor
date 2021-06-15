// microphonePlayground.cpp

#include <Arduino.h>
#include <I2S.h>
#include <CircularBuffer.h>

#define BUFFER_SIZE 1000

//Parameters
const int micPin = A0;

//Variables
CircularBuffer<int, BUFFER_SIZE> mic_readings;
volatile bool buffer_overflow_flag = false;

void measureInterrupt()
{
    // Read the analog value
    int micVal = analogRead(micPin);
    // Store the value, and check if the buffer is full
    if (!mic_readings.push(micVal))
    {
        buffer_overflow_flag = true; // Flag that the buffer is full
    }
}

void setupMicrophone()
{
    pinMode(micPin, INPUT);
    analogReadResolution(16); //resolution is set as high as possible
}

void loopMicrophone()
{
    // Read the collected data and send info the serial prompt
    int sound_level_raw = 0;
    int measurements = mic_readings.size();
    // Safety measure if the buffer is empty (unlikely)
    if (measurements <= 500) // Currently test mode, set == 0 for real
    {
        measureInterrupt(); // Remove after debugging check
        return;
    }
    //Serial.println("Starting data processing");
    while (!mic_readings.isEmpty())
    {
        sound_level_raw += mic_readings.pop();
    }
    float average = (float)sound_level_raw / (float)measurements;
    Serial.print("Measurements: ");
    Serial.print(measurements);
    Serial.print("; Sound level: ");
    Serial.println(average);
}