#include <Arduino.h>
#include <LiquidCrystal.h>

// Initialise the lcd with pins
//LiquidCrystal(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(5, 4, 3, 2, 1, 0);
// Also wire: VDD to 5V, VCC ti GND, RW to GND, V0 to GND

// Custom symbols used in the display:
byte customCharBar0[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00011,
    0b00011,
    0b11011,
    0b11011};

byte customCharBar1[8] = {
    0b00011,
    0b00011,
    0b11011,
    0b11011,
    0b11011,
    0b11011,
    0b11011,
    0b11011};

byte customCharFilled[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111};

// Private variables for this code
unsigned long lcd_current_time, lcd_old_time, lcd_last_updated;
unsigned long lcd_blink_rate = 1000;
int lcd_blink_flag = 0;

void setupLCD()
{
    lcd.begin(16, 2);
    lcd.createChar(0, customCharBar0);
    lcd.createChar(1, customCharBar1);
    lcd.createChar(2, customCharFilled);
    lcd.setCursor(0, 0); // Lcd first row is 0
    lcd.write("Sound! ");
    lcd.write(byte(0));
    lcd.write(byte(1));
}

void loopLCD()
{
    lcd_current_time = millis(); // Update the current time

    // Check if the time elapsed since last update is greater than the "blink_rate"
    if (lcd_current_time - lcd_last_updated > lcd_blink_rate)
    {
        lcd_last_updated = millis(); // Update the timing

        // This part runs through all these 3 states
        if (lcd_blink_flag == 0)
        {
            lcd.setCursor(0, 1); // Lcd first row is 0
            lcd.write(byte(2));
            lcd.write("  ");
            lcd_blink_flag = 1;
        }
        else if (lcd_blink_flag == 1)
        {
            lcd.setCursor(0, 1); // Lcd first row is 0
            lcd.write(byte(2));
            lcd.write(byte(2));
            lcd.write(" ");
            lcd_blink_flag = 2;
        }
        else if (lcd_blink_flag == 2)
        {
            lcd.setCursor(0, 1); // Lcd first row is 0
            lcd.write(byte(2));
            lcd.write(byte(2));
            lcd.write(byte(2));
            lcd_blink_flag = 3;
        }
        else if (lcd_blink_flag == 3)
        {
            lcd.setCursor(0, 1);
            lcd.write("   ");
            lcd_blink_flag = 0;
        }
    }
    // Debugging
    // lcd.setCursor(3, 1);
    // lcd.print(lcd_blink_flag);
}
