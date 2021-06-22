// lcdSoundmeter.h

/*
Defines a class for controlling the 8 LED's in the colorbar.
loopLED can be called in the main loop to periodically turn on the LED's.
There is no fault checking on the initialization, so double check the setup.
*/

#ifndef LED_SOUNDMETER_H
#define LED_SOUNDMETER_H

class LedMeter
{
private:
    int pins[8] = {6, 7, 8, 9, 10, 11, 12, 13}; //Digital pins used for LED
    float max_value = 8; //Max LEDs
    float min_value = 0; //Min LEDs
    // Timing
    unsigned long LED_max_fs, LED_min_fs, LED_last_sample_time; 

public:
    LedMeter(float min_value, float max_value, int pins[8]);
    LedMeter(float min_value, float max_value);
    void write_value(float value);
    float get_max_value();
    float get_max_value_dB();
    void set_max_value(float new_max_value);
    void set_min_value(float new_min_value);
    void set_max_value_dB(float new_max_value_dB);
    bool measure_LED_time = false;
    ~LedMeter(); //Destruct LedMeter
};

// Use this to access the colorbar.
extern LedMeter earMeter;
void setupLED();
void loopLED();

// Don't use these if you can access the class
float getLedMaxValue();
void writeLedCurrentValue(float current_value);

#endif
