/* 
 * File:   mcal_adc.h
 * Author: Amira Hassan
 * 
 * Created on 19 / 9 / 2024, 6:33 am
 * 
 * Description: This file contains the initialization and usage functions 
 *              for the Analog-to-Digital Converter (ADC) using the Hardware 
 *              Abstraction Layer (HAL). The ADC is used to convert analog 
 *              signals (e.g., from sensors) into digital values for processing.
 *  
 */

#ifndef MCAL_ADC_H
#define	MCAL_ADC_H

/* SECTION: Include */
#include "../GPIO/hal_gpio.h"
#include "../INTERRUPT/internal_interrupt.h"

/* SECTION: Macro Declarations */
#define ADC_INPUT_VOLTAGE_FORMAT      0x00
#define ADC_OUTPUT_VOLTAGE_FORMAT     0x01

#define ADC_RESULT_LEFT_FORMAT   0x00
#define ADC_RESULT_RIGH_FORMAT   0x01


/* SECTION: Macro Function Declarations */

/*
 * ref adc_select_channel_t
 */
#define ADC_SELECT_CHANNEL(_channel)  (ADCON0bits.CHS = _channel)

#define ADC_ENABLE()  (ADCON0bits.ADON = 1)
#define ADC_DISABLE()  (ADCON0bits.ADON = 0)

#define ADC_IN_PROGRESS()   (ADCON0bits.GODONE = 1)
//#define ADC_IDLE()   (ADCON0bits.GODONE = 0)
#define ADC_IS_FINISH()   (ADCON0bits.GODONE)

//INPUT => ADD & VSS
#define ADC_INPUT_VOLTAGE()   do{ (ADCON1bits.VCFG0 = 0);\
                                  (ADCON1bits.VCFG1 = 0);\
                                } while(0)
// OUPUT => Vref, ouput battery
#define ADC_OUTPUT_VOLTAGE()   do{ (ADCON1bits.VCFG0 = 1);\
                                   (ADCON1bits.VCFG1 = 1);\
                                } while(0)

/*
 * ref adc_select_analog_t
 */
#define ADC_CONFIG_ANALOG_OR_DIGIT_BIT(_an) {(ADCON1bits.PCFG = _an)}   

/*
 * Result Format LEFT OR RIGHT
 */
#define ADC_RESULT_LEFT()   (ADCON2bits.ADFM = 0)
#define ADC_RESULT_RIGH()   (ADCON2bits.ADFM = 1)

/*
 * ref adc_aquision_time_t
 */
#define ADC_AQUISION_TIME(_TAD) (ADCON2bits.ACQT = _TAD)

/*
 * ref adc_conversion_clock_t
 */
#define ADC_CONVERSION_CLOCK(_FOSC) (ADCON2bits.ADCS = _FOSC)


/* SECTION: Data Type Declarations */
typedef enum {
    ADC_CHANNEL_AN0 = 0,
    ADC_CHANNEL_AN1,
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12,
}adc_select_channel_t;

/**
 * ADC_SELECT_AN8_BIT thats mean from an0 to an8 are analog, and an9 to an12 are digital
 */
typedef enum {
    ADC_SELECT_ANALOG_ALL_BIT = 0x00, // 0x01 // 0x02
    ADC_SELECT_DIGITAL_ALL_BIT = 0x0F,
    ADC_SELECT_AN11_BIT = 0x03,
    ADC_SELECT_AN10_BIT = 0x04,
    ADC_SELECT_AN9_BIT = 0x05,
    ADC_SELECT_AN8_BIT = 0x06,
    ADC_SELECT_AN7_BIT = 0x07,
    ADC_SELECT_AN6_BIT = 0x08,
    ADC_SELECT_AN5_BIT = 0x09,
    ADC_SELECT_AN4_BIT = 0x0A,
    ADC_SELECT_AN3_BIT = 0x0B,
    ADC_SELECT_AN2_BIT = 0x0C,
    ADC_SELECT_AN1_BIT = 0x0D,
    ADC_SELECT_AN0_BIT = 0x0E,
}adc_select_analog_t;

typedef enum{
    ADC_AQUISION_TAD0 = 0,
    ADC_AQUISION_TAD2,
    ADC_AQUISION_TAD4,
    ADC_AQUISION_TAD6,
    ADC_AQUISION_TAD8,
    ADC_AQUISION_TAD10,
    ADC_AQUISION_TAD12,
    ADC_AQUISION_TAD14,
    ADC_AQUISION_TAD16,
    ADC_AQUISION_TAD18,
    ADC_AQUISION_TAD20,
}adc_aquision_time_t;

typedef enum{
    ADC_RC_FOSC_DIV_2 = 0,
    ADC_RC_FOSC_DIV_8,
    ADC_RC_FOSC_DIV_32,
    ADC_RC_FOSC_DIV_4 = 0x04,
    ADC_RC_FOSC_DIV_16,
    ADC_RC_FOSC_DIV_64,
}adc_conversion_clock_t;

typedef struct{
#ifdef ADC_INTERRUPT_FEATURE_ENABLE
    void(*ADC_InterruptHandler)(void);
#ifdef INTERRUPT_PRIORITY_LEVELS
    interrupt_priority priority:1;
#endif
#endif
    adc_select_channel_t channel;
    adc_select_analog_t AN_Bit;
    adc_aquision_time_t TAD_Time;
    adc_conversion_clock_t FOSC_Clock;
    uint8 adc_voltage_reference:1;
    uint8 adc_result_format:1;
    uint8 adc_reserved:6;
}Adc_t;

typedef uint16 adc_result_t;

/* SECTION: Interface Function Declarations */
Std_ReturnType ADC_Init(const Adc_t *_adc);
Std_ReturnType ADC_Dinit(const Adc_t *_adc);
Std_ReturnType ADC_SelectChannel(const Adc_t *_adc, adc_select_channel_t _channel);
Std_ReturnType ADC_StartConversion(const Adc_t *_adc);
Std_ReturnType ADC_IsConversionDone(const Adc_t *_adc, uint8 *status_conversion);
Std_ReturnType ADC_GetConversionResult(const Adc_t *_adc, adc_result_t *conversion_result);
Std_ReturnType ADC_GetConversion_Blocking(const Adc_t *_adc, adc_select_channel_t _channel, adc_result_t *conversion_result);
Std_ReturnType ADC_StartConversionInterrupt(const Adc_t *_adc, adc_select_channel_t _channel);


#endif	/* MCAL_ADC_H */

