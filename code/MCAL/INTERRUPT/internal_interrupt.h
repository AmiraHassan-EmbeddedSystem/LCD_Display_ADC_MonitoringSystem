/* 
 * File:   internal_interrupt.h
 * Author: Amira Hassan
 * 
 * Created on 19 / 9 / 2024, 6:33 am
 * 
 * Description: This file contains the initialization and handling functions 
 *              for internal interrupts using the Hardware Abstraction Layer (HAL).
 *              Internal interrupts are triggered by internal peripherals such as timers,
 *              ADCs, or communication modules (e.g., UART, SPI) to handle specific events 
 *              (e.g., timer overflow, data reception).
 *             
 */

#ifndef INTERNAL_INTERRUPT_H
#define	INTERNAL_INTERRUPT_H

/* SECTION: Include */
#include "manage_interrupt.h"

/* SECTION: Macro Declarations */

/*
 * ADC INTERRUPT
 */
#ifdef ADC_INTERRUPT_FEATURE_ENABLE
    #define ADC_INTERRUPT_ENABLE()    (PIE1bits.ADIE = 1)
    #define ADC_INTERRUPT_DISABLE()    (PIE1bits.ADIE = 0)
    #define ADC_INTERRUPT_CLEAR_FLAG()    (PIR1bits.ADIF = 0)

    #ifdef INTERRUPT_PRIORITY_LEVELS
        #define ADC_INTERRUPT_LOW_PRIORITY()    (IPR1bits.ADIP = 0)
        #define ADC_INTERRUPT_HIGH_PRIORITY()    (IPR1bits.ADIP = 1)
    #endif
#endif



/* SECTION: Macro Function Declarations */
/* SECTION: Data Type Declarations */

/* SECTION: Interface Function Declarations */

#endif	/* INTERNAL_INTERRUPT_H */

