#include <Arduino.h>
#include "ZLG7290.h"

#define SCL                         11
#define SDA                         12

ZLG7290 zlg(SCL, SDA);

void testSegOnOff() {
    unsigned char seg;
    zlg.clear();
    for (seg = 0; seg < 64; seg++)
    {
        zlg.SegOnOff(seg, 1);
        delay(80);
    }
    delay(500);
    for (seg = 0; seg < 64; seg++)
    {
        zlg.SegOnOff(seg, 0);
        delay(80);
    }
    delay(500);
}

void setup()
{
    Serial.begin(9600);
    zlg.begin();
    delay(100);
    zlg.clear();
    zlg.displayNum(9999);           //displayNum(value) value can be -999 to 9999
    delay(2000);                    //If the value is out of range, it will display "----"
    zlg.displayNum(10000);
    delay(2000);
    zlg.displayNum(-999);
    delay(2000);
    zlg.displayNum(-1000);
    delay(2000);
    testSegOnOff();
    zlg.displayNum(10000);
}

void loop()
{
    unsigned char reg1, reg2, reg3;

    zlg.ReadReg(ZLG7290_Key, &reg1);
    zlg.ReadReg(ZLG7290_RepeatCnt, &reg2);
    zlg.ReadReg(ZLG7290_FunctionKey, &reg3);

    Serial.print("KeyValue=");
    Serial.println(reg1);
    Serial.print("KeyRepeatCnt=");
    Serial.println(reg2);
    Serial.print("FunctionKey=");
    Serial.println(reg3);
    delay(500);

}
