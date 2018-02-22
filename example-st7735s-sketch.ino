#include "ST7735S.h"

ST7735S lcd = ST7735S();

void setup()
{
  lcd.begin();
  lcd.clear(ST7735S_WHITE);
  lcd.fillRect(1, 1, 126, 158, ST7735S_GREEN);
}

void loop()
{
}

