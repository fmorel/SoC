/*
	FreeRTOS.org V4.7.0 - Copyright (C) 2003-2007 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	FreeRTOS.org is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FreeRTOS.org; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	A special exception to the GPL can be applied should you wish to distribute
	a combined work that includes FreeRTOS.org, without being obliged to provide
	the source code for any proprietary components.  See the licensing section 
	of http://www.FreeRTOS.org for full details of how and when the exception
	can be applied.

	***************************************************************************
	See http://www.FreeRTOS.org for documentation, latest information, license 
	and contact details.  Please ensure to read the configuration and relevant 
	port sections of the online documentation.

	Also see http://www.SafeRTOS.com a version that has been certified for use
	in safety critical systems, plus commercial licensing, development and
	support options.
	***************************************************************************
*/

#include "FreeRTOS.h"
#include "task.h"

#include "system_conf.h"
#include "Timer.h"
#include "Interrupts.h"


/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the MICO32 port.
 *----------------------------------------------------------*/
#define STACK_INIT_VALUE 0xA5A5A5A5


/*-----------------------------------------------------------*/
static void prvSetupTimer(void);
static void prvTimerInterrupt(unsigned int intrLevel, void *pContext);


/* We require the address of the pxCurrentTCB variable, but don't want to know
any details of its type. */
//TODO Jon: this was commented before
typedef void tskTCB;
extern volatile tskTCB * volatile pxCurrentTCB;

/* ------------------------ Static variables ------------------------------ */
volatile unsigned portLONG              uxCriticalNesting;


/*-----------------------------------------------------------*/



/* 
 * See header file for description. 
 */
portSTACK_TYPE *pxPortInitialiseStack( portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters )
{
	/* Place a 4 bytes of known values on the bottom of the stack. 
	This is just useful for debugging. */
	*pxTopOfStack = 0x12345678;
	pxTopOfStack--;

	/* The compiler expects R0 to be 0. */

	/* Place the parameter on the stack in the expected location. */
 	*pxTopOfStack = ( portSTACK_TYPE ) pvParameters;	/* R1 */
	pxTopOfStack--;

	/* Now the remaining registers. */
	*pxTopOfStack = ( portSTACK_TYPE ) 0x02;	/* R2 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x03;	/* R3 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x04;	/* R4 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x05;	/* R5 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x06;	/* R6 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x07;	/* R7 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x08;	/* R8 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x09;	/* R9 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x10;	/* R10 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x11;	/* R11 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x12;	/* R12 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x13;	/* R13 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x14;	/* R14 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x15;	/* R15 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x16;	/* R16 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x17;	/* R17 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x18;	/* R18 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x19;	/* R19 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x20;	/* R20 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x21;	/* R21 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x22;	/* R22 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x23;	/* R23 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x24;	/* R24 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x25;	/* R25 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x26;	/* R26 */
	pxTopOfStack--;
	*pxTopOfStack = ( portSTACK_TYPE ) 0x27;	/* R27 */
	pxTopOfStack--;

	/* The return address */
    *pxTopOfStack = ( portSTACK_TYPE ) pxCode;
	pxTopOfStack--;

	/* The exception return address - which in this case is the start of the task. */
    *pxTopOfStack = ( portSTACK_TYPE ) pxCode;
	pxTopOfStack--;

	/* Status information. */
	*pxTopOfStack = ( portSTACK_TYPE ) 0x00;
	pxTopOfStack--;

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

portBASE_TYPE xPortStartScheduler( void )
{
	/* Setup the hardware to generate the tick. */
	prvSetupTimer();

	/* Kick off the first task. */
	vStartFirstTask();

	/* Should not get here as the tasks are now running! */
	return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the MICO32 port will get stopped.  */
}
/*-----------------------------------------------------------*/


/*
 * Setup timer to generate a tick interrupt.
 */
static void prvSetupTimer(void)
{
    volatile Timer_t *pTimer;
	
	pTimer = (Timer_t *)TIMER_BASE_ADDRESS;

    /* stop the timer first and ack any pending interrupts */
    pTimer->Control &= TIMER_CONTROL_STOP_BIT_MASK;
    pTimer->Status = 0;

	/* Register Interrupt Service Routine */
	RegisterISR(TIMER_IRQ, (void*)pTimer, prvTimerInterrupt);

	/* start the timer                               */
    pTimer->Period = configCPU_CLOCK_HZ / configTICK_RATE_HZ ;
    pTimer->Control = TIMER_CONTROL_START_BIT_MASK | TIMER_CONTROL_INT_BIT_MASK | TIMER_CONTROL_CONT_BIT_MASK;

}



static void prvTimerInterrupt(unsigned int intrLevel, void *pContext)
{
    Timer_t *pISRTimer = (Timer_t *)pContext;

	#if configUSE_PREEMPTION == 1
	/*
	 * Tick ISR for preemptive scheduler.
	 */
	vTaskIncrementTick();
	vTaskSwitchContext();
	#else
	/*
	 * Tick ISR for the cooperative scheduler.  All this does is increment the
	 * tick count.  We don't need to switch context, this can only be done by
	 * manual calls to taskYIELD();
	 */
	vTaskIncrementTick();
	#endif
	
	/* Clear Timer Status */
    pISRTimer->Status = 0;	
}


/* Critical section management. */
void vPortEnterCritical( void )
{
	/* Port Disable Interrupts */
	portDISABLE_INTERRUPTS();
	uxCriticalNesting++;
}
/*-----------------------------------------------------------*/

void vPortExitCritical( void )
{
 	uxCriticalNesting--;
	if( uxCriticalNesting == 0 )
	{
		portENABLE_INTERRUPTS();
	}
}
