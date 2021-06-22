// cloudPlayground.h

/* 
The connected file handles communication with the arduino coud.
Call loopCloud in the main loop to update the cloud every 5 seconds.
The cloud variables used in this project was a "write" or "get" function for accessing them.
*/

#ifndef CLOUDPLAYGROUND_H
#define CLOUDPLAYGROUND_H
void loopCloud();
void setupCloud();
void write_dB_read(int new_dB_read);
void write_dB_boundary(int new_dB_boundary);
bool get_sleep_val();
void write_sleep_val(bool new_sleep_val);

#endif
