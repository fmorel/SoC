/*
 *
 * SOCLIB_GPL_HEADER_BEGIN
 *
 * This file is part of SoCLib, GNU GPLv2.
 *
 * SoCLib is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * SoCLib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SoCLib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * SOCLIB_GPL_HEADER_END
 *
 * Copyright (c) UPMC, Lip6, SoC
 *         Nicolas Pouillon <nipo@ssji.net>, 2006-2007
 *
 * Maintainers: tarik.graba@telecom-paristech.fr
 */

#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Interrupts.h"
#include "semphr.h"
#include "segmentation.h"

#define VIDEO_OUT   *((volatile unsigned int *)VIDEO_OUT_BASE)
#define VIDEO_IN    *((volatile unsigned int *)VIDEO_IN_BASE)

//in word (4 bytes)
#define WIDTH 160
#define HEIGHT 480
#define MODULO 16

uint32_t image[HEIGHT][WIDTH] = { { 0 } };
uint32_t image2[HEIGHT][WIDTH] = { { 0 } };

/*
   int damier(int i, int j) {
   i = i % MODULO;
   j *= 4;
   j = j % MODULO;
   if (i<MODULO/2 && j<MODULO/2) return 0xffffffff;
   if (i>= MODULO/2 && j< MODULO/2) return 0;
   if (i< MODULO/2 && j>= MODULO/2) return 0;
   return 0xffffffff;
   }
 */
volatile int lock = 0;
xSemaphoreHandle xSemaphore;
void foo(unsigned int intrLevel, void*pContext) {
  my_printf("Yoo!");
  lock = 1;
  static signed portBASE_TYPE xHigherPriorityTaskWoken; 
  /* Unblock the task by releasing the semaphore. */
  xSemaphoreGiveFromISR( xSemaphore, &xHigherPriorityTaskWoken );

  /* If xHigherPriorityTaskWoken was set to true you
     we should yield.  The actual macro used here is 
     port specific. */
  //portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

  if( xHigherPriorityTaskWoken )  vPortYield();

}

void main_task(void *parameters) {
  int i;

  RegisterISR(2,NULL, &foo);
  xSemaphoreTake( xSemaphore, portMAX_DELAY );

  for(i = 0;; i++) {
    VIDEO_IN = (i % 2) == 0 ? (uint32_t)image : (uint32_t)image2;
    my_printf("Trying\n\r");
    while(!lock);
    my_printf("lockTaken\n\r");
    xSemaphoreTake( xSemaphore, portMAX_DELAY );
    my_printf("SemTaken\n\r");
    lock = 0;
    VIDEO_OUT = (i % 2) == 0 ? (uint32_t)image : (uint32_t)image2;

  }

  getchar();
}
int main(void) {
  xTaskHandle my_task;
  vSemaphoreCreateBinary(xSemaphore);
  xTaskCreate(&main_task, (const signed char*)"test task", configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+1,&my_task);
  vTaskStartScheduler();
  return 0;
}

