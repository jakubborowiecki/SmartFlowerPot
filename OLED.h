/*
 * OLED.h
 *
 *  Created on: Sep 10, 2025
 *      Author: kubax
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "main.h"

#define OLED_TimeOut 1000
//#define OLED_USE_DMA

#define OLED_ADDRESS (0x3C)
#define OLED_BUFFER_SIZE (OLED_LCDHEIGHT * OLED_LCDWIDTH / 8)


#define BLACK OLED_BLACK     ///< Draw 'off' pixels
#define WHITE OLED_WHITE     ///< Draw 'on' pixels
#define INVERSE OLED_INVERSE ///< Invert pixels
/// fit into the OLED_ naming scheme
#define OLED_BLACK 0   ///< Draw 'off' pixels
#define OLED_WHITE 1   ///< Draw 'on' pixels
#define OLED_INVERSE 2 ///< Invert pixels

#define OLED_MEMORYMODE 0x20          ///< See datasheet
#define OLED_COLUMNADDR 0x21          ///< See datasheet
#define OLED_PAGEADDR 0x22            ///< See datasheet
#define OLED_SETCONTRAST 0x81         ///< See datasheet
#define OLED_CHARGEPUMP 0x8D          ///< See datasheet
#define OLED_SEGREMAP 0xA0            ///< See datasheet
#define OLED_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define OLED_DISPLAYALLON 0xA5        ///< Not currently used
#define OLED_NORMALDISPLAY 0xA6       ///< See datasheet
#define OLED_INVERTDISPLAY 0xA7       ///< See datasheet
#define OLED_SETMULTIPLEX 0xA8        ///< See datasheet
#define OLED_DISPLAYOFF 0xAE          ///< See datasheet
#define OLED_DISPLAYON 0xAF           ///< See datasheet
#define OLED_COMSCANINC 0xC0          ///< Not currently used
#define OLED_COMSCANDEC 0xC8          ///< See datasheet
#define OLED_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define OLED_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define OLED_SETPRECHARGE 0xD9        ///< See datasheet
#define OLED_SETCOMPINS 0xDA          ///< See datasheet
#define OLED_SETVCOMDETECT 0xDB       ///< See datasheet

#define OLED_SETLOWCOLUMN 0x00  ///< Not currently used
#define OLED_SETHIGHCOLUMN 0x10 ///< Not currently used
#define OLED_SETSTARTLINE 0x40  ///< See datasheet

#define OLED_EXTERNALVCC 0x01  ///< External display voltage source
#define OLED_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define OLED_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define OLED_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define OLED_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define OLED_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define OLED_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range

// Deprecated size stuff for backwards compatibility with old sketches
#define OLED_LCDWIDTH 128 ///< DEPRECATED: width w/OLED_128_64 defined
#define OLED_LCDHEIGHT 64 ///< DEPRECATED: height w/OLED_128_64 defined


void OLED_Init(I2C_HandleTypeDef *i2c);
void OLED_Clear(uint8_t Color);
void OLED_Display(void);
void OLED_DrawPixel(uint16_t x, uint16_t y, uint8_t Color);



#endif /* INC_OLED_H_ */
