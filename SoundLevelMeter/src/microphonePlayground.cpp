// microphonePlayground.cpp

#include <Arduino.h>
#include <I2S.h>
#include <CircularBuffer.h>
#include "LEDSoundmeter.h"
#include "cloudPlayground.h"
#include "lcdSoundmeter.h"
#include <Timer5.h>

#define BUFFER_SIZE 1000

//Parameters
const int micPin = A0;
const int potPin = A2;
int old_pot_reading = 0;
const int MINIMAL_MEASUREMENTS_FOR_DATA_PROCESSING = 200;

//Variables
CircularBuffer<int, BUFFER_SIZE> mic_readings;
CircularBuffer<int, 5> five_max_values;
volatile bool buffer_overflow_flag = false;
volatile bool using_ISP_variable_flag = false;

// Timing
bool measure_dataprocessing = false;
unsigned long data_average_fs, data_max_fs, data_min_fs, data_last_sample_time;

void measureInterrupt()
{
    if (!using_ISP_variable_flag) // Check if the buffer is in use
    {
        // Read the analog value, Range: 0 - 65520
        int micVal = analogRead(micPin);
        // Store the value, and check if the buffer is full
        if (!mic_readings.push(micVal))
        {
            buffer_overflow_flag = true; // Flag that the buffer is full
        }
    }
}

void setupMicrophone()
{
    pinMode(micPin, INPUT);
    analogReadResolution(16); //resolution is set as high as possible
    earMeter.set_max_value(40020);
    earMeter.set_min_value(0);
    // Initialize timer
    MyTimer5.begin(800); // [Hz]
    MyTimer5.attachInterrupt(measureInterrupt);
    MyTimer5.start();
    // Reset timing variables
    if (measure_dataprocessing)
    {
        data_average_fs = 0;
        data_max_fs = 0;
        data_min_fs = INT16_MAX;
        data_last_sample_time = 0;
    }
}

// Read the collected data and send info
void loopMicrophone()
{
    // Loop variables
    int sound_level_raw = 0;
    int measurements = mic_readings.size();
    int current_reading;
    int max = 0;
    int min = INT16_MAX; // Large
    int pot_reading;
    float deviations[5];
    float average;

    //
    // Check entry conditions
    //
    if (measurements < MINIMAL_MEASUREMENTS_FOR_DATA_PROCESSING) // Could be changed.
    {
        return; // Bad style to return in the middle of a function.
    }
    // else -> rest of this function

    // Change the sampling flag
    using_ISP_variable_flag = true;

    //
    // Change output threshold
    //
    analogReadResolution(4); // No need for big fancy precise samples
    pot_reading = analogRead(potPin);
    if (pot_reading != old_pot_reading) // Only update on a change
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

    //
    // Do some data processing
    //
    for (int i = 0; i < 5; i++)
    {
        five_max_values.push(0);
    }

    // Safety measure if the buffer is empty
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
        for (int i = 0; i < 5; i++)
        {
            if (five_max_values[i] < current_reading)
            {
                five_max_values.push(current_reading);
            }
        }
    }
    average = (float)sound_level_raw / (float)measurements;
    for (int i = 0; i < 5; i++)
    {
        deviations[i] = five_max_values.pop() - average;
    }

    // Timing code part 1, currently measureing LCD time
    if (measure_dataprocessing == true)
    {
        data_last_sample_time = micros(); // Update timing variable
    }

    //
    // Output, lcd, led, serial, cloud
    //
    lcd.setCursor(0, 0);
    lcd.print(average);
    lcd.write(byte(0));
    lcd.write(byte(1));

    // Timing code part 2
    if (measure_dataprocessing == true)
    {
        // Timing measurements, measures the time since the last update of data_last_sample_time
        unsigned long current_fs;
        current_fs = micros() - data_last_sample_time;        // Calculate sample time
        data_average_fs = (data_average_fs + current_fs) / 2; // Not really an average
        if (current_fs > data_max_fs)
        {
            data_max_fs = current_fs;
        }
        else if (current_fs < data_min_fs)
        {
            data_min_fs = current_fs;
        }
        Serial.print("Timing, mic, min: ");
        Serial.print(data_min_fs);
        Serial.print("; max: ");
        Serial.print(data_max_fs);
        Serial.print("; Average: ");
        Serial.println(data_average_fs);
    }

    // LED output
    earMeter.write_value(average);

    // Serial output
    /*
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
    Serial.print(earMeter.get_max_value());
    */
    Serial.print("; max_deviation: ");
    Serial.print(max - average);
    Serial.print("; 5 deviations: ");
    Serial.print(deviations[0]);
    Serial.print(" | ");
    Serial.print(deviations[1]);
    Serial.print(" | ");
    Serial.print(deviations[2]);
    Serial.print(" | ");
    Serial.print(deviations[3]);
    Serial.print(" | ");
    Serial.println(deviations[4]);

    // Cloud output
    write_dB_read(average);

    // Set the sampling flag back
    using_ISP_variable_flag = false;
}
