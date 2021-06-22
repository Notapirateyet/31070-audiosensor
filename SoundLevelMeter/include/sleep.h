// sleep.h

/*
Provides a sleep functionality for the system, activated from button input.
"loopSleep" should be called in the main loop.
*/

#ifndef SLEEP_H
#define SLEEP_H
#include <Arduino.h>

void setupSleep(voidFuncPtr main_setup); //setup sleep function
void loopSleep(); //loop function for sleep

#endif
