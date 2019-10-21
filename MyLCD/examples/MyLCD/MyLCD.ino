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

	lcd.LcdDisplay(1,1, "Hello World!");
	delay(1000);
	lcd.clear();
	lcd.LcdDisplay(2, 1, "Schrock000!");
	delay(1000);
	lcd.clear();
}


void loop()
{


}
