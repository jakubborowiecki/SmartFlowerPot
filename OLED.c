/*
 * OLED.c
 *
 *  Created on: Sep 10, 2025
 *      Author: kubax
 */

#include "OLED.h"
#include "main.h"
#include "string.h"
#include "GFX_OLED.h"

I2C_HandleTypeDef *oled_i2c;

static uint8_t buffer[OLED_BUFFER_SIZE];



void OLED_Command(uint8_t Command)
{
	HAL_I2C_Mem_Write(oled_i2c, (OLED_ADDRESS<<1), 0x00, 1, &Command, 1, OLED_TimeOut);
}

void OLED_Data(uint8_t *Data, uint16_t Size)
{
#ifdef OLED_USE_DMA
	if(oled_i2c -> hdmatx -> State == HAL_DMA_STATE_READY)
	{
	HAL_I2C_Mem_Write_DMA(oled_i2c, (OLED_ADDRESS<<1), 0x40, 1, Data, Size);
	}
#else
	HAL_I2C_Mem_Write(oled_i2c, (OLED_ADDRESS<<1), 0x40, 1, Data, Size, OLED_TimeOut);
#endif


}

void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
                                    uint8_t corners, int16_t delta,
                                    uint16_t color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;
  int16_t px = x;
  int16_t py = y;

  delta++; // Avoid some +1's in the loop

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    // These checks avoid double-drawing certain lines, important
    // for the SSD1306 library which has an INVERT drawing mode.
    if (x < (y + 1)) {
      if (corners & 1)
        writeFastVLine(x0 + x, y0 - y, 2 * y + delta, color);
      if (corners & 2)
        writeFastVLine(x0 - x, y0 - y, 2 * y + delta, color);
    }
    if (y != py) {
      if (corners & 1)
        writeFastVLine(x0 + py, y0 - px, 2 * px + delta, color);
      if (corners & 2)
        writeFastVLine(x0 - py, y0 - px, 2 * px + delta, color);
      py = y;
    }
    px = x;
  }
}

void OLED_DrawPixel(uint16_t x, uint16_t y, uint8_t Color)
{
	if ((x < 0) || (x >= OLED_LCDWIDTH) || (y < 0) || (y >= OLED_LCDHEIGHT))
	{
		return;
	}


	switch (Color)
	{
	case OLED_WHITE:
		buffer[x + (y / 8) * OLED_LCDWIDTH] |= (1 << (y & 7));
		break;
	case OLED_BLACK:
		buffer[x + (y / 8) * OLED_LCDWIDTH] &= ~(1 << (y & 7));
		break;
	case OLED_INVERSE:
		buffer[x + (y / 8) * OLED_LCDWIDTH] ^= (1 << (y & 7));
		break;
	}

}








void OLED_Clear(uint8_t Color)
{
	switch(Color)
	{
	case OLED_WHITE:
		memset(buffer, 0xFF, OLED_BUFFER_SIZE);
		break;

	case OLED_BLACK:
		memset(buffer, 0x00, OLED_BUFFER_SIZE);
		break;
	}
}


void OLED_Display(void)
{
	//sekwencja startowa ustawiająca wskaźnik
    OLED_Command(OLED_PAGEADDR);
    OLED_Command(0);                   // Page start address
    OLED_Command(0xFF);                // Page end
    OLED_Command(OLED_COLUMNADDR);     // Column start address
    OLED_Command(0);                   // Page start address
    OLED_Command(OLED_LCDWIDTH - 1);

    OLED_Data(buffer, OLED_BUFFER_SIZE);
}


void OLED_Init(I2C_HandleTypeDef *i2c)
{
	oled_i2c = i2c;
	OLED_Command(OLED_DISPLAYOFF);

	OLED_Command(OLED_SETDISPLAYCLOCKDIV);
	OLED_Command(0x80);

	OLED_Command(OLED_LCDHEIGHT - 1);

	OLED_Command(OLED_SETDISPLAYOFFSET);
	OLED_Command(0x00);
	OLED_Command(OLED_SETSTARTLINE);

	OLED_Command(OLED_CHARGEPUMP);
	OLED_Command(0x14);

	OLED_Command(OLED_MEMORYMODE);
	OLED_Command(0x00);
	OLED_Command(OLED_SEGREMAP | 0x1);
	OLED_Command(OLED_COMSCANDEC);

    OLED_Command(OLED_SETCOMPINS);
    OLED_Command(0x12);
    OLED_Command(OLED_SETCONTRAST);
    OLED_Command(0xFF); // ustawienie jasnosci, 8 bitowa rozdzielczosc

    OLED_Command(OLED_SETPRECHARGE); // 0xd9
    OLED_Command(0xF1);

    OLED_Command(OLED_SETVCOMDETECT); // 0xDB
    OLED_Command(0x40);
    OLED_Command(OLED_DISPLAYALLON_RESUME); // 0xA4
    OLED_Command(OLED_NORMALDISPLAY);       // 0xA6
    OLED_Command(OLED_DEACTIVATE_SCROLL);

    OLED_Command(OLED_DISPLAYON); // Main screen turn on




}
