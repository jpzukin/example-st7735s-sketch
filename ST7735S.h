#include <SPI.h>

#ifndef __ST7735S_H__
#define __ST7735S_H__

/*
 *  System Function Command List 
 */
#define   ST7735S_SWRESET   (0x36)    // Software Reset
#define   ST7735S_SLPOUT    (0x11)    // Sleep Out & Booster On

#define   ST7735S_INVOFF    (0x20)    // Display Inversion Off
#define   ST7735S_INVON     (0x21)    // Display Inversion On

#define   ST7735S_DISPOFF   (0x28)    // Display Off
#define   ST7735S_DISPON    (0x29)    // Display On

#define   ST7735S_MADCTL    (0x36)    // Memory Data Access Control
#define   ST7735S_COLMOD    (0x3A)    // Interface Pixel Format

#define   ST7735S_CASET     (0x2A)    // Column Address Set
#define   ST7735S_RASET     (0x2B)    // Row Address Set
#define   ST7735S_RAMWR     (0x2C)    // Memory Write

/*
 * 色定数 (16bit & RGB)
 */
#define   ST7735S_BLACK     (0b0000000000000000)
#define   ST7735S_RED       (0b1111100000000000)
#define   ST7735S_GREEN     (0b0000011111100000)
#define   ST7735S_BLUE      (0b0000000000011111)
#define   ST7735S_PURPLE    (0b1111100000011111)
#define   ST7735S_YELLOW    (0b1111111111100000)
#define   ST7735S_AQUA      (0b0000011111111111)
#define   ST7735S_WHITE     (0b1111111111111111)

/*
 * 液晶モジュールクラス
 */
class ST7735S
{
  public:
    ST7735S(uint8_t cs=10, uint8_t ds=8, uint8_t rst=9);
  
    void begin();
    void clear(uint16_t color);
    void drawPixel(uint8_t x, uint8_t y, uint16_t color); 
    void fillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);   
  
  /*
   * 本来 private にする所だけど検証のために public とする
   */
    void setDrawArea(uint8_t xs, uint8_t ys, uint8_t xe, uint8_t ye);
  
    void writeCommand(uint8_t data);
    void writeData(uint8_t data);

    inline void enableChip()     { digitalWrite(_csPin, LOW);  }
    inline void disableChip()    { digitalWrite(_csPin, HIGH); }
    inline void setCommandMode() { digitalWrite(_dsPin, LOW);  }
    inline void setDataMode()    { digitalWrite(_dsPin, HIGH); }
  
    SPISettings _SPISettings;
    uint8_t _csPin;         // チップ選択ピン番号
    uint8_t _dsPin;         // コマンド/データ選択ピン番号
    uint8_t _rstPin;        // ハードウェアリセットピン番号
    uint8_t _drawWidth;     // 描画範囲の幅
    uint8_t _drawHeight;    // 描画範囲の高さ
  
    // 製品固定の値
    const uint8_t _offsetX     = 2;   // 描画開始位置列
    const uint8_t _offsetY     = 1;   // 行
    const uint8_t _panelWidth  = 128; // 液晶パネルの幅
    const uint8_t _panelHeight = 160; // 高さ
};

#endif

