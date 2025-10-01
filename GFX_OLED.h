/*
 * GFX_OLED.h
 *
 *  Created on: Sep 11, 2025
 *      Author: kubax
 */

#ifndef INC_GFX_OLED_H_
#define INC_GFX_OLED_H_




#include "main.h"
#include "GFX_OLED.h"
#include "OLED.h"


#define drawPixel(x,y,color) OLED_DrawPixel(x, y, color);
#define WIDTH OLED_LCDWIDTH
#define HEIGHT OLED_LCDHEIGHT
#define PIXEL_BLACK OLED_BLACK
#define PIXEL_WHITE OLED_WHITE
#define PIXEL_INVERSE INVERSE

#define USING_STRINGS 1

#define USING_IMAGE 1

#if USING_IMAGE == 1
#define USING_IMAGE_ROTATE 0
#endif


#define USING_RECTANGLE 1


void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void startWrite(void);

void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);





void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

void endWrite(void);




void writePixel(int16_t x, int16_t y, uint16_t color);

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);

void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
                        uint16_t color);




void drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                     int16_t radius, uint16_t color);
void fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
                     int16_t radius, uint16_t color);

void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                             uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                        uint16_t color);


void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                   int16_t y2, uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                   int16_t y2, uint16_t color);

void Image(int x, int y, const uint8_t *img, uint8_t w, uint8_t h, uint8_t color);
void ImageRotate(int x, int y, const uint8_t *img, uint8_t w, uint8_t h, uint8_t color, uint16_t angle);
double sinus(uint16_t angle);



// fonty i stringi



void SetFont(const uint8_t* font_t);
void SetFontSize(uint8_t size_t);
uint8_t GetFontHeight(void);
uint8_t GetFontWidth(void);
uint8_t GetFontSize(void);


void DrawChar(int x, int y, char chr, uint8_t color, uint8_t background);
void DrawString(int x, int y, char* str, uint8_t color, uint8_t background);




//bitmapa
void Image(int x, int y, const uint8_t *img, uint8_t w, uint8_t h, uint8_t color);





#endif /* INC_GFX_OLED_H_ */
