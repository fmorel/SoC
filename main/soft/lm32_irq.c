/*
 * =============================================================================
 *
 *       Filename:  lm32_irq.c
 *
 *    Description:  LM32 Interruption handling
 *
 *        Version:  1.0
 *       Compiler:  gcc
 *
 *         Author:  Tarik Graba (TG), tarik.graba@telecom-paristech.fr
 *        Company:  Telecom Paris TECH
 *
 *           Note: The code is inspired by the FreeRTOS community contribution
 *                 port for the LM32 done by Klaus Fetsche
 * =============================================================================
 */


#include "lm32_irq.h"

// get cycle counter
unsigned long get_cc ( void )
{
    unsigned long CC;
    asm volatile ("rcsr %0, CC" :"=r"(CC));
    return CC;
}
