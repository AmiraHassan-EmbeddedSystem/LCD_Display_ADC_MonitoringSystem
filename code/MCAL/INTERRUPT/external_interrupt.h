/* 
 * File:   external_interrupt.h
 * Author: Amira Hassan
 * 
 * Created on 19 / 9 / 2024, 6:33 am
 * 
 * Description: This file contains the initialization and handling functions 
 *              for external interrupts using the Hardware Abstraction Layer (HAL).
 *              External interrupts allow the microcontroller to respond immediately
 *              to changes in input signals (e.g., buttons, sensors) without polling.
 *           
 */

#ifndef EXTERNAL_INTERRUPT_H
#define	EXTERNAL_INTERRUPT_H

/* SECTION: Include */
#include "manage_interrupt.h"
//#include "../../"
/* SECTION: Macro Declarations */



/* SECTION: Macro Function Declarations */
#ifdef EXTERNAL_INTERRUPT_INTx_FEATURE
    // INT0 Interrupt Control
    #define EXT_INT0_InterruptEnable()                (INTCONbits.INT0IE = 1)
    #define EXT_INT0_InterruptDisable()               (INTCONbits.INT0IE = 0)
    #define EXT_INT0_InterruptFlagClear()             (INTCONbits.INT0IF = 0)
    #define EXT_INT0_RisingEdgeSet()                  (INTCON2bits.INTEDG0 = 1)
    #define EXT_INT0_FallingEdgeSet()                 (INTCON2bits.INTEDG0 = 0)

    // INT1 Interrupt Control
    #define EXT_INT1_InterruptEnable()                (INTCON3bits.INT1IE = 1)
    #define EXT_INT1_InterruptDisable()               (INTCON3bits.INT1IE = 0)
    #define EXT_INT1_InterruptFlagClear()             (INTCON3bits.INT1IF = 0)
    #define EXT_INT1_RisingEdgeSet()                  (INTCON2bits.INTEDG1 = 1)
    #define EXT_INT1_FallingEdgeSet()                 (INTCON2bits.INTEDG1 = 0)

    // INT2 Interrupt Control
    #define EXT_INT2_InterruptEnable()                (INTCON3bits.INT2IE = 1)
    #define EXT_INT2_InterruptDisable()               (INTCON3bits.INT2IE = 0)
    #define EXT_INT2_InterruptFlagClear()             (INTCON3bits.INT2IF = 0)
    #define EXT_INT2_RisingEdgeSet()                  (INTCON2bits.INTEDG2 = 1)
    #define EXT_INT2_FallingEdgeSet()                 (INTCON2bits.INTEDG2 = 0)

    #ifdef INTERRUPT_PRIORITY_LEVELS
        #define EXT_INT1_InterruptHighPriority()       (INTCON3bits.INT1IP = 1)
        #define EXT_INT1_InterruptLowPriority()        (INTCON3bits.INT1IP = 0)

        #define EXT_INT2_InterruptHighPriority()       (INTCON3bits.INT2IP = 1)
        #define EXT_INT2_InterruptLowPriority()        (INTCON3bits.INT2IP = 0)
    #endif
#endif

#ifdef EXTERNAL_INTERRUPT_RBx_FEATURE
    #define EXT_RBx_InterruptEnable()   (INTCONbits.RBIE = 1)
    #define EXT_RBx_InterruptDisable()  (INTCONbits.RBIE = 0)
    #define EXT_RBx_InterruptFlagClear() (INTCONbits.RBIF = 0)

    #ifdef INTERRUPT_PRIORITY_LEVELS
        #define EXT_RBx_InterruptHighPriority()  (INTCON2bits.RBIP = 1)
        #define EXT_RBx_InterruptLowPriority()   (INTCON2bits.RBIP = 0)
    #endif
#endif


/* SECTION: Data Type Declarations */
typedef void (*InterruptHandler)(void);

typedef enum{
    INTERRUPT_EXTERNAL_INT0 = 0, 
    INTERRUPT_EXTERNAL_INT1, 
    INTERRUPT_EXTERNAL_INT2
}Interrupt_INTx_source;

typedef enum{
    INTERRUPT_FAILING_EDGE = 0,
    INTERRUPT_ERISING_EDGE, 
}Interrupt_INTx_edge;

typedef struct{
    void (*EXT_InterruptHandler)(void);
    pin_config_t mcu_pin;
    interrupt_priority Priority;
    Interrupt_INTx_source    source;
    Interrupt_INTx_edge    edge;
}Ext_INTx_t;

typedef struct{
    void (*EXT_HIGH_interruptHandler)(void);
    void (*EXT_LOW_interruptHandler)(void);
    pin_config_t mcu_pin;
    interrupt_priority Priority;
}Ext_RBx_t;


/* SECTION: Interface Function Declarations */
Std_ReturnType Ext_INTx_Init(const Ext_INTx_t *intx);
Std_ReturnType Ext_INTx_DeInit(const Ext_INTx_t *intx);

Std_ReturnType Ext_RBx_Init(const Ext_RBx_t *rbx);
Std_ReturnType Ext_RBx_DeInit(const Ext_RBx_t *rbx);
#endif	/* EXTERNAL_INTERRUPT_H */

