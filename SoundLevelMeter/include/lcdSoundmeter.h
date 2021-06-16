// lcdSoundmeter.h
#ifndef LCDSOUNDMETER_H
#define LCDSOUNDMETER_H
#include <LiquidCrystal.h>

// Initialise the lcd without pins (see .cpp file)
extern LiquidCrystal lcd;

void setupLCD();
void loopLCD();

#endif