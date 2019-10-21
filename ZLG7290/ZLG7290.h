#pragma once

#include <Arduino.h>

#define I2C_DELAY_VALUE             35
#define I2C_STOP_WAIT_VALUE         1

#define ZLG_7290_I2C_ADDR           0x38

#define ZLG7290_SystemReg           0x00
#define ZLG7290_Key                 0x01
#define ZLG7290_RepeatCnt           0x02
#define ZLG7290_FunctionKey         0x03
#define ZLG7290_CmdBuf              0x07
#define ZLG7290_CmdBuf0             0x07
#define ZLG7290_CmdBuf1             0x08
#define ZLG7290_FlashOnOff          0x0c
#define ZLG7290_ScanNum             0x0d
#define ZLG7290_DpRam               0x10
#define ZLG7290_DpRam0              0x10
#define ZLG7290_DpRam1              0x11
#define ZLG7290_DpRam2              0x12
#define ZLG7290_DpRam3              0x13
#define ZLG7290_DpRam4              0x14
#define ZLG7290_DpRam5              0x15
#define ZLG7290_DpRam6              0x16
#define ZLG7290_DpRam7              0x17

class ZLG7290 {
public:
    ZLG7290(int sclPin, int sdaPin);
    void begin();
    void clear();
    unsigned char WriteReg(unsigned char RegAddr, unsigned char dat);
    unsigned char ReadReg(unsigned char RegAddr, unsigned char *dat);
    unsigned char Command(unsigned char cmd0, unsigned char cmd1);
    unsigned char SegOnOff(unsigned char seg, unsigned char b);
    unsigned char Download(unsigned char addr, unsigned char dp, unsigned char flash, unsigned char dat);
    void displayNum(int data);
    unsigned char getKey();
    void writeMatrix(byte *p);
    void setMatrixXY(unsigned char x, unsigned char y, unsigned char value);
private:
    int _sclPin;
    int _sdaPin;

    void i2c_start();
    void i2c_stop();
    void i2c_writeByte(unsigned char data);
    unsigned char i2c_readByte();
    void i2c_writeAck(bool ack);
    unsigned char i2c_readAck();
    unsigned char Write(unsigned char regAddr, unsigned char *pData, unsigned int Size);
    unsigned char Read(unsigned char regAddr, unsigned char *pData, unsigned int Size);
    
    

};
