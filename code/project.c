/*
 * File:   project.c
 * Author: ah427
 *
 * Created on 21 ?????, 2024, 08:41 ?
 */


#include <xc.h>
//#include <builtins.h>
#include "project.h"
#define _XTAL_FREQ 80000000UL
#pragma config PBADEN = OFF 




void initialization(void);
void app_adc(void);

uint16 adc_res1 = 0, adc_res2 = 0, adc_res3 = 0, adc_res4 = 0;
uint8 adc_res1_text[6], adc_res2_text[6], adc_res3_text[6], adc_res4_text[6];
uint8 ADC_REQ = 0;

lcd_8bit_mode lcd = {
    .rs.Direction = GPIO_OUTPUT,
    .rs.Logic = GPIO_LOW,
    .rs.Port = GPIO_PORTD,
    .rs.Pin = GPIO_PIN0,
    
    .en.Direction = GPIO_OUTPUT,
    .en.Logic = GPIO_LOW,
    .en.Port = GPIO_PORTD,
    .en.Pin = GPIO_PIN1,
    
    .data[0].Direction = GPIO_OUTPUT,
    .data[0].Logic = GPIO_LOW,
    .data[0].Port = GPIO_PORTC,
    .data[0].Pin = GPIO_PIN0,
    
    .data[1].Direction = GPIO_OUTPUT,
    .data[1].Logic = GPIO_LOW,
    .data[1].Port = GPIO_PORTC,
    .data[1].Pin = GPIO_PIN1,
    
    .data[2].Direction = GPIO_OUTPUT,
    .data[2].Logic = GPIO_LOW,
    .data[2].Port = GPIO_PORTC,
    .data[2].Pin = GPIO_PIN2,
    
    .data[3].Direction = GPIO_OUTPUT,
    .data[3].Logic = GPIO_LOW,
    .data[3].Port = GPIO_PORTC,
    .data[3].Pin = GPIO_PIN3,
    
    .data[4].Direction = GPIO_OUTPUT,
    .data[4].Logic = GPIO_LOW,
    .data[4].Port = GPIO_PORTC,
    .data[4].Pin = GPIO_PIN4,
    
    .data[5].Direction = GPIO_OUTPUT,
    .data[5].Logic = GPIO_LOW,
    .data[5].Port = GPIO_PORTC,
    .data[5].Pin = GPIO_PIN5,
    
    .data[6].Direction = GPIO_OUTPUT,
    .data[6].Logic = GPIO_LOW,
    .data[6].Port = GPIO_PORTC,
    .data[6].Pin = GPIO_PIN6,
    
    .data[7].Direction = GPIO_OUTPUT,
    .data[7].Logic = GPIO_LOW,
    .data[7].Port = GPIO_PORTC,
    .data[7].Pin = GPIO_PIN7,
};

Adc_t adc_1= {
//    .ADC_InterruptHandler = app_adc,
    .TAD_Time = ADC_AQUISION_TAD12,
    .FOSC_Clock = ADC_RC_FOSC_DIV_16,
    .channel = ADC_CHANNEL_AN0,
    .adc_result_format = ADC_RESULT_RIGH_FORMAT,
    .adc_voltage_reference = ADC_INPUT_VOLTAGE_FORMAT,
};


void main(void) {
    initialization();
    
    lcd_8bit_send_string_pos(&lcd, "ADC Test", 1, 7);
    __delay_ms(250);
    lcd_8bit_send_command(&lcd, LCD_CLEAR_SCREEN);
        
    lcd_8bit_send_string_pos(&lcd, "Pot1: ", 1, 1);
    lcd_8bit_send_string_pos(&lcd, "Pot2: ", 2, 1);
    lcd_8bit_send_string_pos(&lcd, "Pot3: ", 3, 1);
    lcd_8bit_send_string_pos(&lcd, "Pot4: ", 4, 1);

    while(1){
        if(0 == ADC_REQ){ 
            ADC_REQ = 1;
            ADC_GetConversion_Blocking(&adc_1, ADC_CHANNEL_AN0, &adc_res1);
        }
        else if(1 == ADC_REQ){
            ADC_REQ = 2;
            ADC_GetConversion_Blocking(&adc_1, ADC_CHANNEL_AN1, &adc_res2);  
        }
        else if(2 == ADC_REQ){
            ADC_REQ = 3;
            ADC_GetConversion_Blocking(&adc_1, ADC_CHANNEL_AN2, &adc_res3); 
        }
        else if(3 == ADC_REQ){
            ADC_REQ = 0;
            ADC_GetConversion_Blocking(&adc_1, ADC_CHANNEL_AN3, &adc_res4); 
        }
        else{
            /*Nothing*/
        }
        
        
        lcd_convert_uint16_string(adc_res1, adc_res1_text);
        lcd_convert_uint16_string(adc_res2, adc_res2_text);
        lcd_convert_uint16_string(adc_res3, adc_res3_text);
        lcd_convert_uint16_string(adc_res4, adc_res4_text);

        
        lcd_8bit_send_string_pos(&lcd, adc_res1_text, 1, 7);
        lcd_8bit_send_string_pos(&lcd, adc_res2_text, 2, 7);
        lcd_8bit_send_string_pos(&lcd, adc_res3_text, 3, 7);
        lcd_8bit_send_string_pos(&lcd, adc_res4_text, 4, 7);
    }
    
    return;
}

void initialization(void){
    ADC_Init(&adc_1);
    lcd_8bit_initialize(&lcd);
    
    
}