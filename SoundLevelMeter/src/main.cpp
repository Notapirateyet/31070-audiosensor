#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string>
//#include "InterruptPlayground.h"
#include "microphonePlayground.h"
#include "cloudPlayground.h"

/*LiquidCrystal lcd(10, 11, 6, 7, 8, 9);

//int const potPin = A1;
//int potVal;

void setupDisplay()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0); // Lcd first row is 0
  lcd.print("Hej");
}

void loopDisplay()
{

  potVal = analogRead(potPin);
  Serial.print(potVal);
  Serial.print("\n");
  lcd.setCursor(0, 1); // Lcd first row is 0
  lcd.print(potVal);
  lcd.print(" ");
  //  lcd.setCursor(0,1);
  //  delay(1000);
  //  lcd.print("     ");
}
*/
void setup()
{
  //setupMicrophone();
  setupCloud();
}

void loop()
{
  //loopMicrophone();
  loopCloud();
}