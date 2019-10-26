/*
    Name:       MyLCD1602.ino
    Created:	2019/10/21 21:43:54
    Author:     schrock
*/
#include <Arduino.h>
#include "MyLCD1602.h"

#define		SCL			14
#define		SDA			15

MyLCD1602 lcd (SCL, SDA);

void setup()
{
	lcd.begin();

	lcd.clear();
	delay(100);
	lcd.LcdDisplay(1, 1, "Hello World!    ");
	lcd.LcdDisplay(2, 1, "     ----Schrock");
}


void loop()
{


}
