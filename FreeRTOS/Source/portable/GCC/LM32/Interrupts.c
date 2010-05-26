#include "FreeRTOS.h"
#include "Interrupts.h"

/******************************************************************************
 * Local data structures                                                      *
 ******************************************************************************/

/* ISREntry table */
static ISREntry_t ISREntryTable[MAX_MICO32_ISR_LEVEL+1];


/******************************************************************************
 * Interrupt Handler (invoked by low-level routine)                           *
 * Services interrupts based on priority                                      *
 ******************************************************************************/
void IRQ_handler(void)
{
    /*
     * If an interrupt-handler exists for the relevant interrupt (as detected
     * from ip and im cpu registers), then invoke the handler else disable the
     * interrupt in the im.
     */
    unsigned int ip, im, Mask, IntLevel;
    asm volatile ("rcsr %0,im":"=r"(im));

    do
    {
        /* read ip and calculate effective ip */
        asm volatile ("rcsr %0,ip":"=r"(ip));
        ip &= im;
        Mask = 0x1;
        IntLevel = 0;

        if( ip!=0 )
        {
            do
            {
                if(Mask & ip)
                {
                    if(ISREntryTable[IntLevel].Callback != 0)
                    {
                        (ISREntryTable[IntLevel].Callback)(IntLevel, ISREntryTable[IntLevel].Context);
                        asm volatile ("wcsr ip, %0"::"r"(Mask));
                        break;
                    }
                    else
                    {
                        asm volatile ("rcsr %0,im":"=r"(im));
                        im &= ~Mask;
                        asm volatile ("wcsr im, %0"::"r"(im));
                        asm volatile ("wcsr ip, %0"::"r"(Mask));
                        break;
                    }
                }

                Mask <<= 0x1;
                IntLevel++;
            }
            while(1);
        }
        else
        {
            break;
        }
    }
    while(1);
}



/******************************************************************************
 * Registers ISR callback-routine                                             *
 * Only a single source (of 32) can be registered for.                        *
 * Max IntNum is determined by MAX_ISR_LEVEL and its value goes from 0->n-1   *
 ******************************************************************************/
unsigned int RegisterISR(unsigned int IntNum, void *Context, ISRCallback Callback)
{
    unsigned int Mask;
    unsigned int im;

    if(IntNum > MAX_MICO32_ISR_LEVEL)
    {
        return(pdFAIL);
    }
    
    /* can be optimized by lookup table when not using barrel-shifter */
    Mask = 0x1 << IntNum;

    ISREntryTable[IntNum].Callback = Callback;
    ISREntryTable[IntNum].Context = Context;

    /* mask/unmask bit in the im */
    asm volatile ("rcsr %0, im":"=r"(im));
    im = (Callback == 0)?(im & ~Mask) : (im | Mask);
    asm volatile ("wcsr im, %0"::"r"(im));

    /* all done */
    return(pdPASS);
}



/******************************************************************************
 * Disables a specific interrupt                                              *
 *----------------------------------------------------------------------------*
 * Inputs:                                                                    *
 *     unsigned int IntNum: Interrupt-level                                   *
 * Outputs:                                                                   *
 * Return values:                                                             *
 *            MICO_STATUS_E_INVALID_PARAM                                     *
 *            MICO_STATUS_OK                                                  *
 ******************************************************************************/
unsigned int DisableInterrupt(unsigned int IntNum)
{
    unsigned int im;
    unsigned int Mask;

    if(IntNum > MAX_MICO32_ISR_LEVEL)
    {
        return(pdFAIL);
    }
    
    /* can be optimized by lookup table when not using barrel-shifter */
    Mask = ~(0x1 << IntNum);

    /* disable mask-bit in im */
    asm volatile ("rcsr %0, im":"=r"(im));
    im &= Mask;
    asm volatile ("wcsr im, %0"::"r"(im));

    /* all done */
    return(pdPASS);
}


/******************************************************************************
 * Enables a specific interrupt                                               *
 * ---------------------------------------------------------------------------*
 * Inputs:                                                                    *
 *     unsigned int IntNum: Interrupt-level                                   *
 * Outputs:                                                                   *
 * Return values:                                                             *
 *            MICO_STATUS_E_INVALID_PARAM                                     *
 *            MICO_STATUS_OK                                                  *
 ******************************************************************************/
unsigned int EnableInterrupt(unsigned int IntNum)
{
    unsigned int im;
    unsigned int Mask;

    if(IntNum > MAX_MICO32_ISR_LEVEL)
    {
        return(pdFAIL);
    }

    /* can be optimized by lookup table when not using barrel-shifter */
    Mask = (0x1 << IntNum);

    /* enable mask-bit in im */
    asm volatile ("rcsr %0, im":"=r"(im));
    im |= Mask;
    asm volatile ("wcsr im, %0"::"r"(im));

    /* all done */
    return(pdPASS);
}

