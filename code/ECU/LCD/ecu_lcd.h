/* 
 * File:   ecu_lcd.h
 * Author: ah427
 *
 * Created on 04 ??????, 2024, 06:50 ?
 */


#ifndef ECU_LCD_H
#define	ECU_LCD_H

/* SECTION: Include */
#include "../../MCAL/GPIO/hal_gpio.h"

/* SECTION: Macro Declarations */
#define _XTAL_FREQ 80000000UL


#define LCD_CLEAR_SCREEN                0x01
#define LCD_RETURN_HOME                 0x02
#define LCD_ENTRY_MODE                  0x06
#define LCD_CURSOR_OFF_DISPLAY_ON       0x0C
#define LCD_CURSOR_OFF_DISPLAY_OFF      0x08
#define LCD_SHIFT_RIGHT                 0x1C
#define LCD_SHIFT_LEFT                  0x18
#define LCD_8BIT_2LINE                  0x38
#define LCD_4BIT_2LINE                  0x28
#define LCD_DDRAM_START                 0x80
#define LCD_CGRAM_START                 0x40

#define ROW1    1
#define ROW2    2
#define ROW3    3
#define ROW4    4

/* SECTION: Macro Function Declarations */
/* SECTION: Data Type Declarations */
typedef struct{
    pin_config_t rs;
    pin_config_t en;
    pin_config_t data[4];
}lcd_4bit_mode; 

typedef struct{
    pin_config_t rs;
    pin_config_t en;
    pin_config_t data[8];
}lcd_8bit_mode; 

/* SECTION: Interface Function Declarations */
Std_ReturnType lcd_4bit_initialize(const lcd_4bit_mode *_lcd);
Std_ReturnType lcd_4bit_send_command(const lcd_4bit_mode *_lcd, uint8 command);
Std_ReturnType lcd_4bit_send_char(const lcd_4bit_mode *_lcd, uint8 data);
Std_ReturnType lcd_4bit_send_char_pos(const lcd_4bit_mode *_lcd, uint8 data, uint8 row, uint8 column);
Std_ReturnType lcd_4bit_send_string(const lcd_4bit_mode *_lcd, uint8 *str);
Std_ReturnType lcd_4bit_send_string_pos(const lcd_4bit_mode *_lcd, uint8 *str, uint8 row, uint8 column);

Std_ReturnType lcd_8bit_initialize(const lcd_8bit_mode *_lcd);
Std_ReturnType lcd_8bit_send_command(const lcd_8bit_mode *_lcd, uint8 command);
Std_ReturnType lcd_8bit_send_char(const lcd_8bit_mode *_lcd, uint8 data);
Std_ReturnType lcd_8bit_send_char_pos(const lcd_8bit_mode *_lcd, uint8 data, uint8 row, uint8 column);
Std_ReturnType lcd_8bit_send_string(const lcd_8bit_mode *_lcd, uint8 *str);
Std_ReturnType lcd_8bit_send_string_pos(const lcd_8bit_mode *_lcd, uint8 *str, uint8 row, uint8 column);
Std_ReturnType lcd_8bit_send_custom_char(const lcd_8bit_mode *_lcd, uint8 row, uint8 column, 
        const uint8 *_chr[], uint8 mempos);

Std_ReturnType lcd_convert_uint8_string(uint8 value, uint8 *str);
Std_ReturnType lcd_convert_uint16_string(uint16 value, uint8 *str);
Std_ReturnType lcd_convert_uint32_string(uint32 value, uint8 *str);




#endif	/* ECU_LCD_H */

