#include "external_interrupt.h"

static void (*INT0_InterruptHandler)(void) = NULL;
static void (*INT1_InterruptHandler)(void) = NULL;
static void (*INT2_InterruptHandler)(void) = NULL;

//InterruptHandler INT0_InterruptHandler = NULL;
//InterruptHandler INT1_InterruptHandler  = NULL;
//InterruptHandler INT2_InterruptHandler = NULL;

InterruptHandler RB4_HIGH_InterruptHandler = NULL;
InterruptHandler RB4_LOW_InterruptHandler = NULL;

InterruptHandler RB5_HIGH_InterruptHandler = NULL;
InterruptHandler RB5_LOW_InterruptHandler = NULL;

InterruptHandler RB6_HIGH_InterruptHandler = NULL;
InterruptHandler RB6_LOW_InterruptHandler = NULL;

InterruptHandler RB7_HIGH_InterruptHandler = NULL;
InterruptHandler RB7_LOW_InterruptHandler = NULL;


void ISR_INT0(void);
void ISR_INT1(void);
void ISR_INT2(void);

void ISR_RB4(uint8 RB_source);
void ISR_RB5(uint8 RB_source);
void ISR_RB6(uint8 RB_source);
void ISR_RB7(uint8 RB_source);




static Std_ReturnType Ext_INTx_Disable(const Ext_INTx_t *intx);
static Std_ReturnType Ext_INTx_Enable(const Ext_INTx_t *intx);
static Std_ReturnType Ext_INTx_ClearFlag(const Ext_INTx_t *intx);
static Std_ReturnType Ext_INTx_SetEdge(const Ext_INTx_t *intx);
static Std_ReturnType Ext_INTx_Priority(const Ext_INTx_t *intx);

static Std_ReturnType INT0_setInterruptHandler(void (*interruptHandler)(void));
static Std_ReturnType INT1_setInterruptHandler(void (*interruptHandler)(void));
static Std_ReturnType INT2_setInterruptHandler(void (*interruptHandler)(void));
static Std_ReturnType Ext_INTx_setInterruptHandler(const Ext_INTx_t *intx);


static Std_ReturnType Ext_RBx_Disable(const Ext_RBx_t *rbx);
static Std_ReturnType Ext_RBx_Enable(const Ext_RBx_t *rbx);
static Std_ReturnType Ext_RBx_ClearFlag(const Ext_RBx_t *rbx);
static Std_ReturnType Ext_RBx_Priority(const Ext_RBx_t *rbx);

static Std_ReturnType RB4_High_setInterruptHandler(void (*interruptHandler)(void));
static Std_ReturnType RB4_Low_setInterruptHandler(void (*interruptHandler)(void));

static Std_ReturnType RB5_High_setInterruptHandler(void (*interruptHandler)(void));
static Std_ReturnType RB5_Low_setInterruptHandler(void (*interruptHandler)(void));

static Std_ReturnType RB6_High_setInterruptHandler(void (*interruptHandler)(void));
static Std_ReturnType RB6_Low_setInterruptHandler(void (*interruptHandler)(void));

static Std_ReturnType RB7_High_setInterruptHandler(void (*interruptHandler)(void));
static Std_ReturnType RB7_Low_setInterruptHandler(void (*interruptHandler)(void));
static Std_ReturnType Ext_RBx_setInterruptHandler(const Ext_RBx_t *rbx);




Std_ReturnType Ext_INTx_Init(const Ext_INTx_t *intx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == intx){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Interrupt*/
        ret = Ext_INTx_Disable(intx);
        /*clear flag*/
        ret &= Ext_INTx_ClearFlag(intx);
        /*set edge*/
        ret &= Ext_INTx_SetEdge(intx);
        /*priority*/
        #ifdef INTERRUPT_PRIORITY_LEVELS
        Interrupt_priorityLevelEnable();
        ret &= Ext_INTx_Priority(intx);
        #endif
        /*pin config*/
        ret &= gpio_pin_initialize(&(intx->mcu_pin));
        /*interrupt callback*/
        ret &= Ext_INTx_setInterruptHandler(intx);
        /*Enable Interrupt*/
        ret &= Ext_INTx_Enable(intx);
    }
    return ret;
}

Std_ReturnType Ext_INTx_DeInit(const Ext_INTx_t *intx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == intx){
        ret = E_NOT_OK;
    }
    else{
       ret = Ext_INTx_Disable(intx);
         
    }
    return ret;
}


Std_ReturnType Ext_RBx_Init(const Ext_RBx_t *rbx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rbx){
        ret = E_NOT_OK;
    }
    else{
        /*Disable Interrupt*/
        ret = Ext_RBx_Disable(rbx);
        /*clear flag*/
        ret &= Ext_RBx_ClearFlag(rbx);
        /*priority*/
        ret &= Ext_RBx_Priority(rbx);
        /*pin config*/
        ret &= gpio_pin_initialize(&(rbx->mcu_pin));
        /*interrupt callback*/
        ret &= Ext_RBx_setInterruptHandler(rbx);
        /*Enable Interrupt*/
        ret &= Ext_RBx_Enable(rbx);
    }
    return ret;
}

Std_ReturnType Ext_RBx_DeInit(const Ext_RBx_t *rbx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rbx){
        ret = E_NOT_OK;
    }
    else{
        ret = Ext_RBx_Disable(rbx);  
    }
    return ret;
}


static Std_ReturnType Ext_INTx_Disable(const Ext_INTx_t *intx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == intx){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        switch(intx->source){
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_InterruptDisable();
                break;
            case INTERRUPT_EXTERNAL_INT1:
                EXT_INT1_InterruptDisable();
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptDisable();
                break;
            default:
                ret = E_NOT_OK;
//                break;
        }
         
    }
    return ret;
}


static Std_ReturnType Ext_INTx_Enable(const Ext_INTx_t *intx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == intx){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
         switch(intx->source){
            case INTERRUPT_EXTERNAL_INT0:
#ifdef INTERRUPT_PRIORITY_LEVELS
                Interrupt_priorityLevelEnable();
                Interrupt_HighPriorityEnable();
#else
                Interrupt_globalInterruptEnable();
                Interrupt_prepheralInterruptEnable();
#endif
                EXT_INT0_InterruptEnable();
                break;
                
            case INTERRUPT_EXTERNAL_INT1:
#ifdef INTERRUPT_PRIORITY_LEVELS
                Interrupt_priorityLevelEnable();
                if(HIGH_PRIORITY == intx->Priority){
                    Interrupt_HighPriorityEnable();
                }
                else if(LOW_PRIORITY == intx->Priority){
                    Interrupt_LowPriorityEnable();
                }
                else{
                    /*Nothing*/
                }
#else
                Interrupt_globalInterruptEnable();
                Interrupt_prepheralInterruptEnable();
#endif
                EXT_INT1_InterruptEnable();
                break;
                
            case INTERRUPT_EXTERNAL_INT2:
#ifdef INTERRUPT_PRIORITY_LEVELS
                if(HIGH_PRIORITY == intx->Priority){
                    Interrupt_HighPriorityEnable();
                }
                else if(LOW_PRIORITY == intx->Priority){
                    Interrupt_LowPriorityEnable();
                }
                else{
                    /*Nothing*/
                }
#else
                Interrupt_globalInterruptEnable();
                Interrupt_prepheralInterruptEnable();
#endif
                EXT_INT2_InterruptEnable();
                break;
            default:
                ret = E_NOT_OK;
        }       
         
    }
    return ret;
}

static Std_ReturnType Ext_INTx_ClearFlag(const Ext_INTx_t *intx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == intx){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
         switch(intx->source){
            case INTERRUPT_EXTERNAL_INT0:
                EXT_INT0_InterruptFlagClear();
                break;
            case INTERRUPT_EXTERNAL_INT1:
                EXT_INT1_InterruptFlagClear();
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptFlagClear();
                break;
            default:
                ret = E_NOT_OK;
        }
         
    }
    return ret;
}

static Std_ReturnType Ext_INTx_SetEdge(const Ext_INTx_t *intx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == intx){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
         switch(intx->source){
            case INTERRUPT_EXTERNAL_INT0:
                if(INTERRUPT_ERISING_EDGE == intx->edge)
                    EXT_INT0_RisingEdgeSet();
                else if(INTERRUPT_FAILING_EDGE == intx->edge)
                    EXT_INT0_FallingEdgeSet();
                else{
                    /*Nothing*/
                }
                break;
                
            case INTERRUPT_EXTERNAL_INT1:
                if(INTERRUPT_ERISING_EDGE == intx->edge)
                    EXT_INT1_RisingEdgeSet();
                else if(INTERRUPT_FAILING_EDGE == intx->edge)
                    EXT_INT0_FallingEdgeSet();
                else{
                    /*Nothing*/
                }
                break;
                
            case INTERRUPT_EXTERNAL_INT2:
                if(INTERRUPT_ERISING_EDGE == intx->edge)
                    EXT_INT2_RisingEdgeSet();
                else if(INTERRUPT_FAILING_EDGE == intx->edge)
                    EXT_INT0_FallingEdgeSet();
                else{
                    /*Nothing*/
                }
                break;
                
            default:
                /*Nothing*/
                break;
        }
         
    }
    return ret;
}
#ifdef INTERRUPT_PRIORITY_LEVELS
static Std_ReturnType Ext_INTx_Priority(const Ext_INTx_t *intx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == intx){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
         switch(intx->source){ 
            case INTERRUPT_EXTERNAL_INT0:
//                Interrupt_priorityLevelEnable();

                break;
            case INTERRUPT_EXTERNAL_INT1:
//                Interrupt_priorityLevelEnable();
                if(HIGH_PRIORITY == intx->Priority)
                    EXT_INT1_InterruptHighPriority();
                else if(LOW_PRIORITY == intx->Priority)
                    EXT_INT1_InterruptLowPriority();
                else{
                    /*Nothing*/
                }
                break;
                
            case INTERRUPT_EXTERNAL_INT2:
                if(HIGH_PRIORITY == intx->Priority)
                    EXT_INT2_InterruptHighPriority();
                else if(LOW_PRIORITY == intx->Priority)
                    EXT_INT2_InterruptLowPriority();
                else{
                    /*Nothing*/
                }
                break;
                
            default:
                /*Nothing*/
                break;
        }
         
    }
    return ret;
}
#endif

static Std_ReturnType INT0_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        INT0_InterruptHandler = interruptHandler;
    }
    return ret;
}


static Std_ReturnType INT1_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        INT1_InterruptHandler = interruptHandler;
    }
    return ret;
}


static Std_ReturnType INT2_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        INT2_InterruptHandler = interruptHandler;
    }
    return ret;
}

static Std_ReturnType Ext_INTx_setInterruptHandler(const Ext_INTx_t *intx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == intx){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
         switch(intx->source){
            case INTERRUPT_EXTERNAL_INT0:
                INT0_setInterruptHandler(intx->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT1:
                INT1_setInterruptHandler(intx->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT2:
                INT2_setInterruptHandler(intx->EXT_InterruptHandler);
                break;
            default:
                /*Nothing*/
                break;
        }
         
    }
    return ret;
}


static Std_ReturnType Ext_RBx_Disable(const Ext_RBx_t *rbx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rbx){
        ret = E_NOT_OK;
    }
    else{
        ret = EXT_RBx_InterruptDisable();
         
    }
    return ret;
}

static Std_ReturnType Ext_RBx_Enable(const Ext_RBx_t *rbx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rbx){
        ret = E_NOT_OK;
    }
    else{
        ret &= EXT_RBx_InterruptEnable();
    }
    return ret;
}

static Std_ReturnType Ext_RBx_ClearFlag(const Ext_RBx_t *rbx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rbx){
        ret = E_NOT_OK;
    }
    else{
        ret = EXT_RBx_InterruptFlagClear();    
    }
    return ret;
}


static Std_ReturnType Ext_RBx_Priority(const Ext_RBx_t *rbx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rbx){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
#ifdef INTERRUPT_PRIORITY_LEVELS  
        Interrupt_priorityLevelEnable();
        if(HIGH_PRIORITY == rbx->Priority){
            Interrupt_HighPriorityEnable();
            EXT_RBx_InterruptHighPriority();
        }
        else if(LOW_PRIORITY == rbx->Priority){
            Interrupt_LowPriorityEnable();
            EXT_RBx_InterruptLowPriority();
        }
        else{
            /*Nothing*/
        }
#else         
        Interrupt_globalInterruptEnable();
        Interrupt_prepheralInterruptEnable();
        
    
#endif
    }
    return ret;
}

static Std_ReturnType RB4_High_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
         RB4_HIGH_InterruptHandler = interruptHandler;
    }
    return ret;
}

static Std_ReturnType RB4_Low_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
         RB4_LOW_InterruptHandler = interruptHandler;
    }
    return ret;
}


static Std_ReturnType RB5_High_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
         RB5_HIGH_InterruptHandler = interruptHandler;
    }
    return ret;
}

static Std_ReturnType RB5_Low_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
        RB5_LOW_InterruptHandler = interruptHandler;
    }
    return ret;
}

static Std_ReturnType RB6_High_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
        RB6_HIGH_InterruptHandler = interruptHandler;
    }
    return ret;
}

static Std_ReturnType RB6_Low_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
         RB6_LOW_InterruptHandler = interruptHandler;
    }
    return ret;
}

static Std_ReturnType RB7_High_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
         RB7_HIGH_InterruptHandler = interruptHandler;
    }
    return ret;
}

static Std_ReturnType RB7_Low_setInterruptHandler(void (*interruptHandler)(void))
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == interruptHandler){
        ret = E_NOT_OK;
    }
    else{
         RB7_LOW_InterruptHandler = interruptHandler;
    }
    return ret;
}

static Std_ReturnType Ext_RBx_setInterruptHandler(const Ext_RBx_t *rbx)
{
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rbx){
        ret = E_NOT_OK;
    }
    else{
        ret = E_OK;
        switch(rbx->mcu_pin.Pin){
            case GPIO_PIN4:
                ret &= RB4_High_setInterruptHandler(rbx->EXT_HIGH_interruptHandler);
                ret &= RB4_Low_setInterruptHandler(rbx->EXT_LOW_interruptHandler);
                break;
                
            case GPIO_PIN5:
                ret &= RB5_High_setInterruptHandler(rbx->EXT_HIGH_interruptHandler);
                ret &= RB5_Low_setInterruptHandler(rbx->EXT_LOW_interruptHandler);
                break;
                
            case GPIO_PIN6:
                ret &= RB6_High_setInterruptHandler(rbx->EXT_HIGH_interruptHandler);
                ret &= RB6_Low_setInterruptHandler(rbx->EXT_LOW_interruptHandler);
                break;
                
            case GPIO_PIN7:
                ret &= RB7_High_setInterruptHandler(rbx->EXT_HIGH_interruptHandler);
                ret &= RB7_Low_setInterruptHandler(rbx->EXT_LOW_interruptHandler);
                break;
                
            default:
                /*Nothing*/
                break;
        }
         
    }
    return ret;
}

void ISR_INT0(void){
    EXT_INT0_InterruptFlagClear();
    
    if(INT0_InterruptHandler){
        INT0_InterruptHandler();
    }
    else{
        /*nothing*/
    }
}

void ISR_INT1(void){
    EXT_INT1_InterruptFlagClear();
    if(INT1_InterruptHandler){
        INT1_InterruptHandler();
    }
    else{
        /*nothing*/
    }
}

void ISR_INT2(void){
    EXT_INT0_InterruptFlagClear();
    if(INT2_InterruptHandler){
        INT2_InterruptHandler();
    }
    else{
        /*nothing*/
    }
}

void ISR_RB4(uint8 RB_source){
    EXT_RBx_InterruptFlagClear();
    if((0 == RB_source) && (RB4_LOW_InterruptHandler)){
        RB4_LOW_InterruptHandler();
    }
    else if((1 == RB_source) && (RB4_HIGH_InterruptHandler)){
        RB4_HIGH_InterruptHandler();
    }
    else{
        /*nothing*/
    }
}

void ISR_RB5(uint8 RB_source){
    EXT_RBx_InterruptFlagClear();
    if((0 == RB_source) && (RB5_LOW_InterruptHandler)){
        RB5_LOW_InterruptHandler();
    }
    else if((1 == RB_source) && (RB5_HIGH_InterruptHandler)){
        RB5_HIGH_InterruptHandler();
    }
    else{
        /*nothing*/
    }
}
void ISR_RB6(uint8 RB_source){
    EXT_RBx_InterruptFlagClear();
    if((0 == RB_source) && (RB6_LOW_InterruptHandler)){
        RB6_LOW_InterruptHandler();
    }
    else if((1 == RB_source) && (RB6_HIGH_InterruptHandler)){
        RB6_HIGH_InterruptHandler();
    }
    else{
        /*nothing*/
    }
}

void ISR_RB7(uint8 RB_source){
    EXT_RBx_InterruptFlagClear();
    if((0 == RB_source) && (RB7_LOW_InterruptHandler)){
        RB7_LOW_InterruptHandler();
    }
    else if((1 == RB_source) && (RB7_HIGH_InterruptHandler)){
        RB7_HIGH_InterruptHandler();
    }
    else{
        /*nothing*/
    }
}