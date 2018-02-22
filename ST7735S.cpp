#include <SPI.h>
#include "ST7735S.h"

/*
 * csPin: チップ選択ピン番号
 * dsPin: コマンド/データ選択ピン番号
 * rstPin: ハードウェアリセットピン番号
 */
ST7735S::ST7735S(uint8_t csPin, uint8_t dsPin, uint8_t rstPin)
{
  _csPin  = csPin;
  _dsPin  = dsPin;
  _rstPin = rstPin;
  _drawWidth  = _panelWidth;
  _drawHeight = _panelHeight;

  _SPISettings = SPISettings(8000000, MSBFIRST, SPI_MODE0);
}

void ST7735S::begin()
{
  SPI.begin();
  SPI.beginTransaction(_SPISettings);

  pinMode(_csPin,  OUTPUT);
  pinMode(_dsPin,  OUTPUT);
  pinMode(_rstPin, OUTPUT);
   
  // ハードウェアリセット
  enableChip();
  digitalWrite(_rstPin, HIGH);  delay(500);
  digitalWrite(_rstPin, LOW);   delay(500);
  digitalWrite(_rstPin, HIGH);  delay(500);  
  disableChip();

  // リセット状態からの状態変更
  writeCommand(ST7735S_SLPOUT);   delay(150);   // 1. スリープ解除
  writeCommand(ST7735S_DISPON);   delay(150);   // 2. 表示オン
  writeCommand(ST7735S_MADCTL);                 // 3. 書き込み方向:180°、色の並び:RGB
  writeData(0xC0);
  writeCommand(ST7735S_COLMOD);                 // 4. 色深度:16bpp
  writeData(0x05);
}

/*
 * 液晶モジュールへコマンドの送信
 */
void ST7735S::writeCommand(uint8_t data)
{
  enableChip();
  setCommandMode();
  SPI.transfer(data);
  disableChip();
}

/*
 * 液晶モジュールへパラメータ/表示データの送信
 */
void ST7735S::writeData(uint8_t data)
{
  enableChip();
  setDataMode();
  SPI.transfer(data);
  disableChip();
}

void ST7735S::drawPixel(uint8_t x, uint8_t y, uint16_t color)
{
  setDrawArea(x, y, 1, 1);
  writeData(highByte(color));
  writeData( lowByte(color));
}

void ST7735S::setDrawArea(uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye)
{
  writeCommand(ST7735S_CASET);
  writeData(0x00);  writeData(_offsetX + xs);   // 開始列
  writeData(0x00);  writeData(_offsetX + xe);   // 終了列
  writeCommand(ST7735S_RASET);
  writeData(0x00);  writeData(_offsetY + ys);   // 開始行
  writeData(0x00);  writeData(_offsetY + ye);   // 終了行

  writeCommand(ST7735S_RAMWR);  // 描画データ送信開始
}

void ST7735S::clear(uint16_t color)
{
  fillRect(0, 0, _drawWidth, _drawHeight, color);
}

void ST7735S::fillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color)
{
  setDrawArea(x, y, (x + width - 1), (y + height - 1));

  int totalPixel = width * height;
  while (totalPixel > 0) {
     writeData(highByte(color));
     writeData( lowByte(color));
     totalPixel -= 1;
  }
}

