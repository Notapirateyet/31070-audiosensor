// microphonePlayground.cpp
//Source file that uses the microphone input, processes it in a circular buffer, and calculates the average noise value in dB
//The function then sends the output in dB to the cloud, the serial port and the LCD

#include <Arduino.h>
#include <I2S.h>
#include <CircularBuffer.h>
#include "LEDSoundmeter.h"
#include "cloudPlayground.h"
#include "lcdSoundmeter.h"
#include <Timer5.h>

#define BUFFER_SIZE 1000 //Size of the buffer, this can be changed
#define MOVING_AVERAGE_SIZE 4 //Size of the average of the measurements

//Parameters
const int micPin = A0; //Microphone input pin - Analog with ADC capabilities
const int potPin = A2; //Potentiometer input pin - Analog with ADC capabilities
int old_pot_reading = 0; 
const int MINIMAL_MEASUREMENTS_FOR_DATA_PROCESSING = 500; //500 measurements before data is processed

//Variables
CircularBuffer<int, BUFFER_SIZE> mic_readings; //Circular buffer 
CircularBuffer<int, 5> five_max_values;
CircularBuffer<float, MOVING_AVERAGE_SIZE> mv_average_output;
float last_mv_average = 0;
volatile bool buffer_overflow_flag = false; //Owerflow flag is set if buffer is full
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
            buffer_overflow_flag = true; // Flag that is true the buffer is full
        }
    }
}

void setupMicrophone()
{
    pinMode(micPin, INPUT); //Microphone readings are set as an input
    analogReadResolution(16); //resolution is set as high as possible 
    earMeter.set_max_value(40020);
    earMeter.set_min_value(10000);
    // Initialize timer
    MyTimer5.begin(800); // [Hz] Sampling frequency
    MyTimer5.attachInterrupt(measureInterrupt); //Start interrupt routine
    MyTimer5.start();
    // Reset timing variables
    if (measure_dataprocessing)
    {
        data_average_fs = 0;
        data_max_fs = 0;
        data_min_fs = INT16_MAX;
        data_last_sample_time = micros();
    }
    // Preallocate and fill moving average with 0
    for (int i = 0; i < MOVING_AVERAGE_SIZE; i++)
    {
        mv_average_output.push(0);
    }
    lcd.setCursor(14, 0);
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.setCursor(0, 1); //Write to LCD
    lcd.print("Max: ");
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
    float average;
    float measured_dB;
    unsigned long current_fs;

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
        //pot_reading = pot_reading * 1800; // Approximation
        int pot_reading_shifted = pot_reading << 11;
        earMeter.set_max_value(pot_reading_shifted);
        write_dB_boundary(earMeter.get_max_value_dB());
    }

    analogReadResolution(16); // Back to old resolution
    // Timing code part 1, currently measuring data processing time
    if (measure_dataprocessing == true)
    {
        data_last_sample_time = micros(); // Update timing variable
    }
    //
    // Do data processing
    //
    for (int i = 0; i < 5; i++)
    {
        five_max_values.push(0); //calculate the max values
    }

    // Safety measure if the buffer is empty
    while (!mic_readings.isEmpty())
    {
        current_reading = mic_readings.shift();
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
    average = (float)sound_level_raw / (float)measurements;

    // Moving average average calculation
    int mv_buffer_size = mv_average_output.size();
    if (mv_buffer_size > 0)
    {
        last_mv_average = last_mv_average + ((average - mv_average_output.shift()) / (float)mv_buffer_size);
        mv_average_output.push(average);
    }
    else
    {
        last_mv_average = -1;
    }

    // Convert to dB
    measured_dB = 0.0095 * last_mv_average - 114.57;

    // Timing code part 2,
    if (measure_dataprocessing == true)
    {
        current_fs = micros() - data_last_sample_time; // Calculate sample time
    }

    //
    // Output, lcd, led, serial, cloud
    //
    lcd.setCursor(0, 0);
    lcd.print((int)measured_dB);
    lcd.print(" dB ");
    lcd.setCursor(5, 1);
    //    lcd.print("Max: ");
    lcd.print((int)earMeter.get_max_value_dB());
    lcd.print(" dB");

    // Timing code part 2
    if (measure_dataprocessing == true)
    {
        // Timing measurements, measures the time since the last update of data_last_sample_time
        // unsigned long current_fs;
        // current_fs = micros() - data_last_sample_time;        // Calculate sample time
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
        Serial.print("; current: ");
        Serial.print(current_fs);
        Serial.print("; average: ");
        Serial.println(data_average_fs);
    }

    // LED output
    earMeter.write_value(last_mv_average);

    // Serial output

    //    Serial.print("Samples: ");
    //    Serial.print(measurements);
    //    Serial.print("; Min: ");
    //    Serial.print(min);
    Serial.print("; Max: ");
    Serial.print(max);
    Serial.print("; mv_average: ");
    Serial.print(last_mv_average);
    Serial.print("; dB: ");
    Serial.print(measured_dB);
    //output to serial monitor - used for debugging
    Serial.print("; mv_diff: ");
    Serial.print(max - last_mv_average);
    Serial.print("; diff: ");
    Serial.print(max - average);
    Serial.print("; potm: ");
    Serial.print(pot_reading);
    Serial.print("; Max LED: ");
    Serial.println(earMeter.get_max_value());

    /*
    Serial.print("; max_deviation: ");
    Serial.print(max - last_mv_average);
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
    */

    // Cloud output
    write_dB_read(measured_dB);

    // Set the sampling flag back
    using_ISP_variable_flag = false;
}
