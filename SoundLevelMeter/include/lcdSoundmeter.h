// lcdSoundmeter.h

/*
The connected file handles the initialisation of the LCD screen.
The object "lcd" can be used to write directly to the LCD screen.
Do not run the loopLCD function in the main loop, it was created for debugging and initial tests.
*/

#ifndef LCDSOUNDMETER_H
#define LCDSOUNDMETER_H
#include <LiquidCrystal.h>

// Initialise the lcd without pins (see .cpp file)
extern LiquidCrystal lcd;

void setupLCD();
void loopLCD();

#endif