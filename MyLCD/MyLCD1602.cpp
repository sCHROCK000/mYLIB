#include "MyLCD1602.h"

char CGRAM_data[] = {
0x08,0x1F,0x02,0x0F,0x0A,0x1F,0x02,0x02, //��
0X55,0X55,0X55,0X55,0X55,0X55,0X55,0X55, //ż��
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA, //����
0XFF,0X00,0XFF,0X00,0XFF,0X00,0XFF,0X00, //���
0X00,0XFF,0X00,0XFF,0X00,0XFF,0X00,0XFF, //ż��
0XFF,0X11,0X11,0X11,0X11,0X11,0X11,0XFF, //����
0XFF,0X11,0X11,0X11,0X11,0X11,0X11,0XFF, //����
0XFF,0X11,0X11,0X11,0X11,0X11,0X11,0XFF, //����
};

char CGRAM_data_nian[] = {
0x08,0x1F,0x02,0x0F,0x0A,0x1F,0x02,0x02,
};

MyLCD1602::MyLCD1602(int sclPin, int sdaPin) {
	_sclPin = sclPin;
	_sdaPin = sdaPin;
}

void MyLCD1602::begin() {
	pinMode(_sclPin, OUTPUT);
	pinMode(_sdaPin, OUTPUT);
	digitalWrite(_sclPin, HIGH);
	digitalWrite(_sdaPin, HIGH);
	delay(2);
	initLCD();
	delay(2);
};

void MyLCD1602::clear() {
	initLCD();
}

void MyLCD1602::iic_start() {
	digitalWrite(_sclPin, HIGH);
	//delayMicroseconds(delayTime);
	digitalWrite(_sdaPin, HIGH);
	//delayMicroseconds(delayTime);
	digitalWrite(_sdaPin, LOW);
};

void MyLCD1602::iic_stop() {
	digitalWrite(_sclPin, HIGH);
	//delayMicroseconds(delayTime);
	digitalWrite(_sdaPin, LOW);
	//delayMicroseconds(delayTime);
	digitalWrite(_sdaPin, HIGH);
};

void MyLCD1602::iic_write(u8 data) {
	int i;
	digitalWrite(_sclPin, LOW);
	for (i = 0; i < 8; i++)
	{
		
		if (data & 0x80)
			digitalWrite(_sdaPin, HIGH);
		else
			digitalWrite(_sdaPin, LOW);
		digitalWrite(_sclPin, HIGH);
		//delayMicroseconds(delayTime);
		digitalWrite(_sclPin, LOW);
		//delayMicroseconds(delayTime);
		data <<= 1;
	}
	digitalWrite(_sdaPin, LOW);
	digitalWrite(_sclPin, HIGH);
	digitalWrite(_sclPin, LOW);
};

void MyLCD1602::initLCD() {
	delay(100);

	iic_start();
	iic_write(SlaveAddr);
	iic_write(0x00);
	iic_write(0x38);
	iic_write(0x0c);
	iic_write(0x01);
	iic_write(0x06);
	iic_stop();
}

void MyLCD1602::writeCGRAM() {
	int i;
	char *p = CGRAM_data;
	iic_start();

	iic_write(SlaveAddr);
	iic_write(0x80);
	iic_write(0x40);
	iic_write(0x40);
	for (i = 0; i < 64; i++)
	{
		iic_write(*p);
		p++;
	}

	iic_stop();
}

void MyLCD1602::displayCGRAM() {
	int i;
	iic_start();

	iic_write(SlaveAddr);
	iic_write(0x80);
	iic_write(0x80);
	iic_write(0x40);
	for (i = 0; i <16; i++)
	{
		iic_write(0x01);
	}
	iic_stop();

	iic_start();
	iic_write(SlaveAddr);
	iic_write(0x80);
	iic_write(0xc0);
	iic_write(0x40);
	for (i = 0; i < 16; i++)
	{
		iic_write(0x01);
	}

	iic_stop();
}

void MyLCD1602::LcdDisplay(int y, int x, const char *p) {
	int i;
	int ssize = sizeof(p);
	char *pstring = const_cast<char*>(p);
	
	iic_start();
	iic_write(SlaveAddr);
	iic_write(0x80);
	iic_write(0x80+(y-1)*0x40+(x-1));
	iic_write(0x40);
	//memset(pstring, 0, 16);
	for (i = 0; i < 16; i++)
	{
		iic_write(*p);
		p++;
	}
	iic_stop();
}
