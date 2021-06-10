#include "LEDSoundmeter.h"
#include <Arduino.h>


void setupLED()
{
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
}

void loopLED()
{
  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  
  digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  
  digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  
  digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);  
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);   

}