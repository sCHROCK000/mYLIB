#include "ZLG7290.h"

ZLG7290::ZLG7290(int sclPin, int sdaPin) {
    _sclPin = sclPin;
    _sdaPin = sdaPin;
}

void ZLG7290::i2c_start() {
    digitalWrite(_sdaPin, HIGH);
    delayMicroseconds(I2C_DELAY_VALUE);
    digitalWrite(_sclPin, HIGH);
    delayMicroseconds(I2C_DELAY_VALUE);
    digitalWrite(_sdaPin, LOW);
    delayMicroseconds(I2C_DELAY_VALUE);
    digitalWrite(_sclPin, LOW);
    delayMicroseconds(I2C_DELAY_VALUE);
}

void ZLG7290::i2c_stop() {
    digitalWrite(_sdaPin, LOW);
    delayMicroseconds(I2C_DELAY_VALUE);
    digitalWrite(_sclPin, HIGH);
    delayMicroseconds(I2C_DELAY_VALUE);
    digitalWrite(_sdaPin, HIGH);
    delayMicroseconds(I2C_DELAY_VALUE);
    delayMicroseconds(20);
}

void ZLG7290::i2c_writeByte(unsigned char data) {
    int i = 8;

    while(i--)
    {
        if (data & 0x80)
            digitalWrite(_sdaPin, HIGH);
        else
            digitalWrite(_sdaPin, LOW);
        data <<= 1;
        digitalWrite(_sclPin, HIGH);
        delayMicroseconds(I2C_DELAY_VALUE);
        digitalWrite(_sclPin, LOW);
        delayMicroseconds(I2C_DELAY_VALUE);
    }
}

unsigned char ZLG7290::i2c_readByte() {
    unsigned char i = 8;
    unsigned char data;

    digitalWrite(_sdaPin, HIGH);
    pinMode(_sdaPin, INPUT);
    while (i--)
    {
        digitalWrite(_sclPin, HIGH);
        delayMicroseconds(I2C_DELAY_VALUE);
        data <<= 1;
        if (digitalRead(_sdaPin))
            data |= 0x01;
        digitalWrite(_sclPin, LOW);
        delayMicroseconds(I2C_DELAY_VALUE);
        
    }
    pinMode(_sdaPin, INPUT);
    return data;
    
}

void ZLG7290::i2c_writeAck(bool ack) {
    if (ack)
        digitalWrite(_sdaPin, HIGH);
    else
        digitalWrite(_sdaPin, LOW);
    delayMicroseconds(I2C_DELAY_VALUE);
    digitalWrite(_sclPin, HIGH);
    delayMicroseconds(I2C_DELAY_VALUE);
    digitalWrite(_sclPin, LOW);
    delayMicroseconds(I2C_DELAY_VALUE);

}

unsigned char ZLG7290::i2c_readAck() {
    unsigned char ack;
    pinMode(_sdaPin, INPUT);
    digitalWrite(_sdaPin, HIGH);
    delayMicroseconds(I2C_DELAY_VALUE);
    digitalWrite(_sclPin, HIGH);
    delayMicroseconds(I2C_DELAY_VALUE);
    ack = digitalRead(_sdaPin);
    digitalWrite(_sclPin, LOW);
    delayMicroseconds(I2C_DELAY_VALUE);
    pinMode(_sdaPin, OUTPUT);
    return ack;

}

unsigned char ZLG7290::Write(unsigned char regAddr, unsigned char *pData, unsigned int Size) {

    if (Size == 0)
        return 0;
    i2c_start();
    i2c_writeByte(ZLG_7290_I2C_ADDR<<1);
    if (i2c_readAck())
    {
        i2c_stop();
        return 0;
    }
    i2c_writeByte(regAddr);
    if (i2c_readAck())
    {
        i2c_stop();
        return 0;
    }
    do
    {
        i2c_writeByte(*pData++);
        if(i2c_readAck())
            break;

    } while (--Size != 0);

    i2c_stop();
    if (Size == 0)
        return 0;
    else
        return 1;

}

unsigned char ZLG7290::Read(unsigned char regAddr, unsigned char *pData, unsigned int Size) {
    if (Size == 0)
        return 0;
    i2c_start();
    i2c_writeByte(ZLG_7290_I2C_ADDR<<1);
    if (i2c_readAck())
    {
        i2c_stop();
        return 1;
    }
    i2c_writeByte(regAddr);
    if (i2c_readAck())
    {
        i2c_stop();
        return 1;
    }

    i2c_start();
    i2c_writeByte(ZLG_7290_I2C_ADDR << 1|0x01);
    if (i2c_readAck())
    {
        i2c_stop();
        return 1;
    }
    for (;;)
    {
        *pData++ = i2c_readByte();
        if (--Size == 0)
        {
            i2c_writeAck(true);
            break;
        }
        i2c_writeAck(false);
    }
    i2c_stop();
    return 0;

}

unsigned char ZLG7290::WriteReg(unsigned char RegAddr, unsigned char dat) {
    unsigned char b;
    b = Write(RegAddr, &dat, 1);
    return b;
}

unsigned char ZLG7290::ReadReg(unsigned char RegAddr, unsigned char *dat) {
    unsigned char b;
    b = Read(RegAddr, dat, 1);
    return b;
}

unsigned char ZLG7290::Command(unsigned char cmd0, unsigned char cmd1) {
    unsigned char b;
    unsigned char buf[2];
    buf[0] = cmd0;
    buf[1] = cmd1;
    b = Write(ZLG7290_CmdBuf, buf, 2);
    return b;

}

unsigned char ZLG7290::SegOnOff(unsigned char seg, unsigned char b) {
    unsigned char cmd = seg & 0x3f;
    if (b)
        cmd |= 0x80;
    return Command(0x01, cmd);
}

unsigned char ZLG7290::Download(unsigned char addr, unsigned char dp, unsigned char flash, unsigned char dat) {
    unsigned char cmd0, cmd1;
    cmd0 = addr & 0x0f;
    cmd0 |= 0x60;
    cmd1 |= dat & 0x1f;
    if (dp)
        cmd1 |= 0x80;
    if (flash)
        cmd1 |= 0x40;
    return Command(cmd0, cmd1);
}

void ZLG7290::clear() {
    unsigned char x;
    for (x = 0; x < 8; x++)
    {
        Download(x, 0, 0, 31);
    }
}

void ZLG7290::displayNum(int data) {
    int a, b, c, d;
    clear();
    if (data >= 0 && data < 10000)
    {
        a = data / 1000;
        Download(0, 0, 0, a);
        b = (data-1000*a)/100;
        Download(1, 0, 0, b);
        c = (data - 1000 * a - 100 * b) / 10;
        Download(2, 0, 0, c);
        d = data%10;
        Download(3, 0, 0, d);
    }
    else if (data < 0 && data > -1000)
    {
        data = ~(data - 1);
        SegOnOff(6, 1);
        a = data / 100;
        Download(1, 0, 0, a);
        b = (data - 100 * a) / 10;
        Download(2, 0, 0, b);
        c = data%10;
        Download(3, 0, 0, c);
    }
    else
    {
        for(int i=0;i<8;i++)
            SegOnOff(6+i*8, 1);
    }
}

unsigned char ZLG7290::getKey() {
    unsigned char KeyValue;
    ReadReg(ZLG7290_Key, &KeyValue);
    return KeyValue;
}

void ZLG7290::setMatrixXY(unsigned char x, unsigned char y, unsigned char value) {
    unsigned char seg;
    seg = x*8+y;
    if(value == 0)
    {
	SegOnOff(seg, 1);
    }
    else
    {
	SegOnOff(seg, 0);
    }
}

void ZLG7290::writeMatrix(byte *p) {
    byte tmp;
    for (int i = 0; i < 8; i++)
    {
        Download(i, 1, 0, 8);
    }

    for(int i=0;i<8;i++)
    {
	tmp = *p;
	p++;
	for(int j=0;j<8;j++)
	{
	    if(tmp&0x01)
	    {
		SegOnOff((i*8+j), 0);
	    }
	    else
	    {
		SegOnOff((i*8+j), 1);
	    }
	    tmp>>=1;
	}
    }
}


void ZLG7290::begin() {
    pinMode(_sclPin, OUTPUT);
    pinMode(_sdaPin, OUTPUT);
    digitalWrite(_sclPin, HIGH);
    digitalWrite(_sdaPin, HIGH);
    delay(2);

}
