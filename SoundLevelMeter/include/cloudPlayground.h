// cloudPlayground.h
#ifndef CLOUDPLAYGROUND_H
#define CLOUDPLAYGROUND_H
void loopCloud();
void setupCloud();

//Get and write functions that passes variables locally
void write_dB_read(int new_dB_read);
void write_dB_boundary(int new_dB_boundary);
bool get_sleep_val();
void write_sleep_val(bool new_sleep_val);

#endif
