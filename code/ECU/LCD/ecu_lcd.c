#include "ecu_lcd.h"
#include <string.h>
#include <stdio.h>

static Std_ReturnType lcd_send_4bit(const lcd_4bit_mode *_lcd, uint8 data);
static Std_ReturnType lcd_4bit_enable_signal(const lcd_4bit_mode *_lcd);
static Std_ReturnType lcd_4bit_set_cursor(const lcd_4bit_mode *_lcd, uint8 row, uint8 column);

static Std_ReturnType lcd_8bit_enable_signal(const lcd_8bit_mode *_lcd);
static Std_ReturnType lcd_8bit_set_cursor(const lcd_8bit_mode *_lcd, uint8 row, uint8 column);


Std_ReturnType lcd_4bit_initialize(const lcd_4bit_mode *_lcd)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_initialize(&(_lcd->rs));
        ret &= gpio_pin_initialize(&(_lcd->en));
        for(uint8 count=0; count<4; count++){
            ret &= gpio_pin_initialize(&(_lcd->data[count]));
        }
        
        __delay_ms(20);
        ret &= lcd_4bit_send_command(_lcd, LCD_8BIT_2LINE);
        __delay_ms(5);
        ret &= lcd_4bit_send_command(_lcd, LCD_8BIT_2LINE);
        __delay_us(150);
        ret &= lcd_4bit_send_command(_lcd, LCD_8BIT_2LINE);
        
        ret &= lcd_4bit_send_command(_lcd, LCD_CLEAR_SCREEN);
        ret &= lcd_4bit_send_command(_lcd, LCD_RETURN_HOME);
        ret &= lcd_4bit_send_command(_lcd, LCD_ENTRY_MODE);
        ret &= lcd_4bit_send_command(_lcd, LCD_CURSOR_OFF_DISPLAY_ON);
        ret &= lcd_4bit_send_command(_lcd, LCD_4BIT_2LINE);
        ret &= lcd_4bit_send_command(_lcd, LCD_DDRAM_START);     
    }
    return ret;
}


Std_ReturnType lcd_4bit_send_command(const lcd_4bit_mode *_lcd, uint8 command)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(_lcd->rs), GPIO_LOW);
        ret &= lcd_send_4bit(_lcd, (command >> 4));
        ret &= lcd_4bit_enable_signal(_lcd);
        ret &= lcd_send_4bit(_lcd, command);
        ret &= lcd_4bit_enable_signal(_lcd);
    }
    return ret;
}


Std_ReturnType lcd_4bit_send_char(const lcd_4bit_mode *_lcd, uint8 data)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(_lcd->rs), GPIO_HIGH);
        ret &= lcd_send_4bit(_lcd, (data >> 4));
        ret &= lcd_4bit_enable_signal(_lcd);
        ret &= lcd_send_4bit(_lcd, data);
        ret &= lcd_4bit_enable_signal(_lcd);
    }
    return ret;
}


Std_ReturnType lcd_4bit_send_char_pos(const lcd_4bit_mode *_lcd, uint8 data, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_set_cursor(_lcd, row, column);
        ret &= lcd_4bit_send_char(_lcd, data);
    }
    return ret;
}


Std_ReturnType lcd_4bit_send_string(const lcd_4bit_mode *_lcd, uint8 *str)
{
    Std_ReturnType ret = E_OK;
    if((NULL == _lcd) && (NULL == str)){
        ret = E_NOT_OK;
    }
    else{
        while(*str){
            ret &= lcd_4bit_send_char(_lcd, *str++);
        }
    }
    return ret;
}


Std_ReturnType lcd_4bit_send_string_pos(const lcd_4bit_mode *_lcd, uint8 *str, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;
    if((NULL == _lcd) && (NULL == str)){
        ret = E_NOT_OK;
    }
    else{
        ret &= lcd_4bit_set_cursor(_lcd, row, column);
        ret &= lcd_4bit_send_string(_lcd, str);
    }
    return ret;
}



Std_ReturnType lcd_8bit_initialize(const lcd_8bit_mode *_lcd)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_initialize(&(_lcd->rs));
        ret &= gpio_pin_initialize(&(_lcd->en));
        for(uint8 count=0; count<8; count++){
            ret &= gpio_pin_initialize(&(_lcd->data[count]));
        }
        
        __delay_ms(20);
        ret &= lcd_8bit_send_command(_lcd, LCD_8BIT_2LINE);
        __delay_ms(5);
        ret &= lcd_8bit_send_command(_lcd, LCD_8BIT_2LINE);
        __delay_us(150);
        ret &= lcd_8bit_send_command(_lcd, LCD_8BIT_2LINE);
        
        ret &= lcd_8bit_send_command(_lcd, LCD_CLEAR_SCREEN);
        ret &= lcd_8bit_send_command(_lcd, LCD_RETURN_HOME);
        ret &= lcd_8bit_send_command(_lcd, LCD_ENTRY_MODE);
        ret &= lcd_8bit_send_command(_lcd, LCD_CURSOR_OFF_DISPLAY_ON);
        ret &= lcd_8bit_send_command(_lcd, LCD_8BIT_2LINE);
        ret &= lcd_8bit_send_command(_lcd, LCD_DDRAM_START);  
    }
    return ret;
}


Std_ReturnType lcd_8bit_send_command(const lcd_8bit_mode *_lcd, uint8 command)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(_lcd->rs), GPIO_LOW);
        for(uint8 count=0; count<8; count++){
            ret &= gpio_pin_write_logic(&(_lcd->data[count]), (command >> count) & 0x01);
        }
        lcd_8bit_enable_signal(_lcd);
    }
    return ret;
}


Std_ReturnType lcd_8bit_send_char(const lcd_8bit_mode *_lcd, uint8 data)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = gpio_pin_write_logic(&(_lcd->rs), GPIO_HIGH);
        for(uint8 count=0; count<8; count++){
            ret &= gpio_pin_write_logic(&(_lcd->data[count]), (data >> count) & 0x01);
        }
        lcd_8bit_enable_signal(_lcd);
    }
    return ret;
}


Std_ReturnType lcd_8bit_send_char_pos(const lcd_8bit_mode *_lcd, uint8 data, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_set_cursor(_lcd, row, column);
        ret &= lcd_8bit_send_char(_lcd, data);
    }
    return ret;
}


Std_ReturnType lcd_8bit_send_string(const lcd_8bit_mode *_lcd, uint8 *str)
{
    Std_ReturnType ret = E_OK;
    if((NULL == _lcd) && (NULL == str)){
        ret = E_NOT_OK;
    }
    else{
        while(*str){
            ret &= lcd_8bit_send_char(_lcd, *str++);
        }
    }
    return ret;
}


Std_ReturnType lcd_8bit_send_string_pos(const lcd_8bit_mode *_lcd, uint8 *str, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;
    if((NULL == _lcd) && (NULL == str)){
        ret = E_NOT_OK;
    }
    else{
        ret &= lcd_8bit_set_cursor(_lcd, row, column);
        ret &= lcd_8bit_send_string(_lcd, str);
    }
    return ret;
}


Std_ReturnType lcd_8bit_send_custom_char(const lcd_8bit_mode *_lcd, uint8 row, uint8 column,
        const uint8 *_chr[], uint8 mempos)
{
    Std_ReturnType ret = E_OK;
    if((NULL == _lcd) && (NULL == _chr)){
        ret = E_NOT_OK;
    }
    else{
        ret &= lcd_8bit_send_command(_lcd, (LCD_CGRAM_START + (mempos*8)) );
        for(uint8 counter=0; counter<=7; counter++){
            ret &= lcd_8bit_send_char(_lcd, _chr[counter]);
        }
        ret &= lcd_8bit_send_char_pos(_lcd, row, column, mempos);
        
    }
    return ret;
}

Std_ReturnType lcd_convert_uint8_string(uint8 value, uint8 *str)
{
    Std_ReturnType ret = E_OK;
    if (NULL == str) {
        ret = E_NOT_OK;
    } else {
        memset(str, '\0', 4);
        sprintf((char *)str, "%u", value);
    }
    return ret;
}


Std_ReturnType lcd_convert_uint16_string(uint16 value, uint8 *str)
{
    Std_ReturnType ret = E_OK;
    if (NULL == str) {
        ret = E_NOT_OK;
    } else {
        memset(str, '\0', 6);  // Allocate space for 5 digits + null terminator
        sprintf((char *)str, "%u", value);  // Use %u for unsigned integer
    }
    return ret;
}


Std_ReturnType lcd_convert_uint32_string(uint32 value, uint8 *str)
        
{
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str, '\0', 11);
        sprintf(str, '%i', value);
    }
    return ret;
}


static Std_ReturnType lcd_send_4bit(const lcd_4bit_mode *_lcd, uint8 data)
{
    Std_ReturnType ret = E_OK;
    uint8 l_bit_count = 0;
    for(l_bit_count=0; l_bit_count<4; l_bit_count++){
        ret &= gpio_pin_write_logic(&(_lcd->data[l_bit_count]), (data >> l_bit_count) & ((uint8)0x01));
    }
    return ret;
}

static Std_ReturnType lcd_4bit_enable_signal(const lcd_4bit_mode *_lcd)
{
    Std_ReturnType ret = E_OK;
    ret &= gpio_pin_write_logic(&(_lcd->en), GPIO_HIGH);
    __delay_us(5);
    ret &= gpio_pin_write_logic(&(_lcd->en), GPIO_LOW);
    return ret;
}

static Std_ReturnType lcd_4bit_set_cursor(const lcd_4bit_mode *_lcd, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;
    --column;
    switch(row){
        case ROW1:
            ret = lcd_4bit_send_command(_lcd, (0x80 + column));
            break;
            
        case ROW2:
            ret = lcd_4bit_send_command(_lcd, (0xC0 + column));
            break;
            
        case ROW3:
            ret = lcd_4bit_send_command(_lcd, (0x94 + column));
            break;
            
        case ROW4:
            ret = lcd_4bit_send_command(_lcd, (0xD4 + column));
            break;
        
        default:
            ret = E_NOT_OK;
    }
    return ret;
}


static Std_ReturnType lcd_8bit_enable_signal(const lcd_8bit_mode *_lcd)
{
    Std_ReturnType ret = E_OK;
    ret &= gpio_pin_write_logic(&(_lcd->en), GPIO_HIGH);
    __delay_us(5);
    ret &= gpio_pin_write_logic(&(_lcd->en), GPIO_LOW);
    return ret;
}

static Std_ReturnType lcd_8bit_set_cursor(const lcd_8bit_mode *_lcd, uint8 row, uint8 column)
{
    Std_ReturnType ret = E_OK;
    --column;
    switch(row){
        case ROW1:
            ret = lcd_8bit_send_command(_lcd, (0x80 + column));
            break;
            
        case ROW2:
            ret = lcd_8bit_send_command(_lcd, (0xC0 + column));
            break;
            
        case ROW3:
            ret = lcd_8bit_send_command(_lcd, (0x94 + column));
            break;
            
        case ROW4:
            ret = lcd_8bit_send_command(_lcd, (0xD4 + column));
            break;
        
        default:
            ret = E_NOT_OK;
    }
    return ret;
}