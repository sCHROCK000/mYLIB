#pragma once

#include <Arduino.h>

#define		u8				unsigned char
#define		u16				unsigned int

#define		SlaveAddr		0x78
#define		delayTime		20

class MyLCD1602 {
public:
	MyLCD1602(int sclPin, int sdaPin);
	void begin();
	void clear();
	void writeCGRAM();
	void displayCGRAM();
	void LcdDisplay(int y, int x, const char *p);

private:
	int _sclPin;
	int _sdaPin;

	void iic_start();
	void iic_stop();
	void iic_write(u8 data);
	void initLCD();
	
};
