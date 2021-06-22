// microphonePLayground.h

/*
The connected class is responsible for measuring sound.
It will call most other functions whenever it is done data processing.
"loopMicrophone" should be run in the main loop.

*/

#ifndef MICROPHONEPLAYGROUND_H
#define MICROPHONEPLAYGROUND_H

void setupMicrophone();
void loopMicrophone();
// This provides a way to  not do analog measurements without disabling interrupts. (0.87 ms @ 800 Hz)
extern volatile bool using_ISP_variable_flag;

#endif