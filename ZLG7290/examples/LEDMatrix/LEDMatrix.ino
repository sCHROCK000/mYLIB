#include <Arduino.h>
#include "ZLG7290.h"

#define SCL                         14
#define SDA                         15

ZLG7290 zlg(SCL, SDA);

byte DBuffer[8] = {
  B10010010,
  B00010010,
  B00110010,
  B00010010,
  B00010010,
  B00010010,
  B00010010,
  B00010010,
};

byte blank[8] = {0};

void setup()
{
  Serial.begin(9600);
  zlg.begin();
  delay(100);
  zlg.writeMatrix(blank);
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      zlg.setMatrixXY(i, j, 1);
      delay(50);
    }
  }
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      zlg.setMatrixXY(i, j, 0);
      delay(50);
    }
  }
  delay(500);
  zlg.writeMatrix(DBuffer);
  delay(2000);
}

void loop()
{

}
