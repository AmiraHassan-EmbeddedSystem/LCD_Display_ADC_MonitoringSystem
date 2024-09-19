#include "manage_interrupt.h"

#ifdef INTERRUPT_PRIORITY_LEVELS
void __interrupt() InterruptManagerHigh(void)
{
    
#ifdef ADC_INTERRUPT_FEATURE_ENABLE
    if((INTERRUPT_ENABLE == PIE1bits.ADIE) &&(INTERRUPT_OCCUR == PIR1bits.ADIF)){
        ISR_ADC();
    }
#endif
}

void __interrupt(low_priority) InterruptManagerLow(void)
{   
#ifdef ADC_INTERRUPT_FEATURE_ENABLE
    if((INTERRUPT_ENABLE == PIE1bits.ADIE) &&(INTERRUPT_OCCUR == PIR1bits.ADIF)){
        ISR_ADC();
    }
    
#endif
}

#else

void  __interrupt() InterruptManagerHigh(void)
{   
#ifdef ADC_INTERRUPT_FEATURE_ENABLE
    if((INTERRUPT_ENABLE == PIE1bits.ADIE) &&(INTERRUPT_OCCUR == PIR1bits.ADIF)){
        ISR_ADC();
    }
#endif
}
#endif
