// microphonePlayground.cpp

#include <Arduino.h>
#include <I2S.h>
#include <CircularBuffer.h>
#include "LEDSoundmeter.h"
#include "cloudPlayground.h"
#include "lcdSoundmeter.h"

#define BUFFER_SIZE 1000

//Parameters
const int micPin = A0;
const int potPin = A2;
int old_pot_reading = 0;

//Variables
CircularBuffer<int, BUFFER_SIZE> mic_readings;
volatile bool buffer_overflow_flag = false;

void measureInterrupt()
{
    // Read the analog value, Range: 0 - 65520
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
    earMeter.set_max_value(40020);
    earMeter.set_min_value(0);
}

void loopMicrophone()
{
    // Read the collected data and send info the serial prompt
    int sound_level_raw = 0;
    int measurements = mic_readings.size();
    int current_reading;
    int max = 0;
    int min = 10000; // Large
    int pot_reading;
    // Safety measure if the buffer is empty (unlikely)
    if (measurements < 1000) // Currently test mode, set == 0 for real
    {
        measureInterrupt(); // Remove after debugging check
        return;
    }
    //Serial.println("Starting data processing");
    // Change output threshold
    analogReadResolution(4); // No need for big fancy precise samples
    pot_reading = analogRead(potPin);
//    if (old_pot_reading - pot_reading >= 0x0010 || old_pot_reading - pot_reading <= -0x0010)
    if (pot_reading != old_pot_reading)
    {
        // Update vars
        old_pot_reading = pot_reading;
        // Scale the potentiometer reading to be the same scale as the microphone reading
        //pot_reading = pot_reading * 900; // Approximation
        int pot_reading_shifted = pot_reading << 10;
        earMeter.set_max_value(pot_reading_shifted);
        write_dB_boundary(earMeter.get_max_value()); 
    }

    analogReadResolution(16); // Back to old resolution

    // Do some data processing
    while (!mic_readings.isEmpty())
    {
        current_reading = mic_readings.pop();
        sound_level_raw += current_reading;
        if (max < current_reading)
        {
            max = current_reading;
        }
        if (min > current_reading)
        {
            min = current_reading;
        }
    }
    float average = (float)sound_level_raw / (float)measurements;
    earMeter.write_value(average);
    Serial.print("Measurements: ");
    Serial.print(measurements);
    Serial.print("; Min: ");
    Serial.print(min);
    Serial.print("; Max: ");
    Serial.print(max);
    Serial.print("; Average level: ");
    Serial.print(average);
    Serial.print("; Potmeter: ");
    Serial.print(pot_reading);

    Serial.print("; Max LED: ");
    Serial.println(earMeter.get_max_value());

    lcd.setCursor(0,0);
    lcd.print(average);

    write_dB_read(average);

}
