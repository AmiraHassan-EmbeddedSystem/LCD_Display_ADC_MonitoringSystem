#include "mcal_adc.h"


void (* InterruptHandler)(void) = NULL;
void ISR_ADC(void);

static inline void adc_input_channel_port_configure(adc_select_channel_t channel);
static inline void adc_voltage_reference(const Adc_t *_adc);
static inline void adc_result_format(const Adc_t *_adc);


Std_ReturnType ADC_Init(const Adc_t *_adc){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        /*Disable The ADC*/
        ADC_DISABLE();
        
        /*Configure The Acquision Time*/
        ADC_AQUISION_TIME(_adc->TAD_Time);
        
        /*Configure The Conversion Cock*/
        ADC_CONVERSION_CLOCK(_adc->FOSC_Clock);
        
        /*Configure The Default Channel*/
        ADC_SELECT_CHANNEL(_adc->channel);
        adc_input_channel_port_configure(_adc->channel);
        
        /*Configure The Interrupt*/ 
#ifdef ADC_INTERRUPT_FEATURE_ENABLE
        ADC_INTERRUPT_ENABLE();
        ADC_INTERRUPT_CLEAR_FLAG();
    #ifdef INTERRUPT_PRIORITY_LEVELS
        Interrupt_priorityLevelEnable();
        if(HIGH_PRIORITY == _adc->priority){
            Interrupt_HighPriorityEnable();
            ADC_INTERRUPT_HIGH_PRIORITY();
        }
        else if(LOW_PRIORITY == _adc->priority){
            Interrupt_LowPriorityEnable();
            ADC_INTERRUPT_LOW_PRIORITY();
        }
        else{
            ret = E_NOT_OK;
        }   
    #else
        Interrupt_globalInterruptEnable();
        Interrupt_prepheralInterruptEnable();
    #endif
        InterruptHandler = _adc->ADC_InterruptHandler;
#endif
        
        /*Configure The Result Format*/
        adc_result_format(_adc);
        
        /*Configure The Voltage Reference*/
        adc_voltage_reference(_adc);
        
        /*Enable The ADC*/
        ADC_ENABLE();
    }
    return ret;
}

Std_ReturnType ADC_Dinit(const Adc_t *_adc){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        ADC_DISABLE();
        
    }
    return ret;
}

Std_ReturnType ADC_SelectChannel(const Adc_t *_adc, adc_select_channel_t _channel){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        ADC_SELECT_CHANNEL(_channel);
        adc_input_channel_port_configure(_channel);   
    }
    return ret;
}

Std_ReturnType ADC_StartConversion(const Adc_t *_adc){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        ADC_IN_PROGRESS();
    }
    return ret;
}

Std_ReturnType ADC_IsConversionDone(const Adc_t *_adc, uint8 *status_conversion)
{
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _adc) || (NULL == status_conversion)){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        *status_conversion = (uint8)(!ADC_IS_FINISH());
    }
    return ret;
}

Std_ReturnType ADC_GetConversionResult(const Adc_t *_adc, adc_result_t *conversion_result)
{
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _adc) || (NULL == conversion_result)){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        switch(_adc->adc_result_format){
            case ADC_RESULT_LEFT_FORMAT:
                *conversion_result = (adc_result_t)((ADRESH >> 8) + (ADRESL >> 6)); 
                break;

            case ADC_RESULT_RIGH_FORMAT:
                *conversion_result = (adc_result_t)((ADRESH >> 8) + ADRESL);
                break;

            default:
                *conversion_result = (adc_result_t)((ADRESH >> 8) + (ADRESL >> 6)); 
        }
    }
    return ret;
}

Std_ReturnType ADC_GetConversion_Blocking(const Adc_t *_adc, adc_select_channel_t _channel, adc_result_t *conversion_result)
{
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _adc) || (NULL == conversion_result)){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        ADC_SELECT_CHANNEL(_channel);
        adc_input_channel_port_configure(_channel);
        ADC_StartConversion(_adc);
        while(ADC_IS_FINISH());
        ADC_GetConversionResult(_adc, conversion_result);
        
    }
    return ret;
}

Std_ReturnType ADC_StartConversionInterrupt(const Adc_t *_adc, adc_select_channel_t _channel)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        ADC_SELECT_CHANNEL(_channel);
        adc_input_channel_port_configure(_channel);
        ADC_StartConversion(_adc);
    }
    return ret;
}

static inline void adc_input_channel_port_configure(adc_select_channel_t channel)
{
    switch (channel){
        case ADC_CHANNEL_AN0: 
            SET_BIT(TRISA, _TRISA_RA0_POSN); 
            break;
            
        case ADC_CHANNEL_AN1:
            SET_BIT(TRISA, _TRISA_RA1_POSN);
            break;
            
        case ADC_CHANNEL_AN2: 
            SET_BIT(TRISA, _TRISA_RA2_POSN); 
            break;
            
        case ADC_CHANNEL_AN3:
            SET_BIT(TRISA, _TRISA_RA3_POSN);
            break;
            
        case ADC_CHANNEL_AN4: 
            SET_BIT(TRISA, _TRISA_RA5_POSN); 
            break;
            
        case ADC_CHANNEL_AN5:
            SET_BIT(TRISE, _TRISE_RE0_POSN);
            break;
            
        case ADC_CHANNEL_AN6: 
            SET_BIT(TRISE, _TRISE_RE1_POSN); 
            break;
            
        case ADC_CHANNEL_AN7:
            SET_BIT(TRISE, _TRISE_RE2_POSN);
            break;
            
        case ADC_CHANNEL_AN8: 
            SET_BIT(TRISB, _TRISB_RB2_POSN); 
            break;
            
        case ADC_CHANNEL_AN9:
            SET_BIT(TRISB, _TRISB_RB3_POSN);
            break;
            
        case ADC_CHANNEL_AN10: 
            SET_BIT(TRISB, _TRISB_RB1_POSN); 
            break;
            
        case ADC_CHANNEL_AN11:
            SET_BIT(TRISB, _TRISB_RB4_POSN);
            break;
            
        case ADC_CHANNEL_AN12:
            SET_BIT(TRISB, _TRISB_RB0_POSN);
            break;
            
        default:
            /*Nothing*/
            break;
    }
}

static inline void adc_voltage_reference(const Adc_t *_adc)
{
    switch(_adc->adc_voltage_reference){
        case ADC_OUTPUT_VOLTAGE_FORMAT:
            ADC_OUTPUT_VOLTAGE();
            break;
            
        case ADC_INPUT_VOLTAGE_FORMAT:
            ADC_INPUT_VOLTAGE();
            break;
            
        default:
            ADC_INPUT_VOLTAGE();
            
    }
}

static inline void adc_result_format(const Adc_t *_adc)
{
    switch(_adc->adc_result_format){
        case ADC_RESULT_LEFT_FORMAT:
            ADC_RESULT_LEFT();
            break;
            
        case ADC_RESULT_RIGH_FORMAT:
            ADC_RESULT_RIGH();
            break;
        
        default:
            ADC_RESULT_RIGH();
    }
}


#ifdef ADC_INTERRUPT_FEATURE_ENABLE
void ISR_ADC(void){
    ADC_INTERRUPT_CLEAR_FLAG();
    if(InterruptHandler){
        InterruptHandler();
    }
}
#endif
