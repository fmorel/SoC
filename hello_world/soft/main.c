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


#include <stdio.h>
#include "Timer.h"
#include "my_printf.h"
#include "FreeRTOS.h"
#include "task.h"

#define WB_TARGET 0xA0000200
#define WB_TIMER 0xA0000600

#define N 10

volatile static signed char buffer[1000]= {0};
int fibo(int n);
int toto;
void stop(int n) {
  int i;
  for (i = 0;i<n;i++) {
    asm volatile("nop");
  }
}

void task(void *parameters)
{
  //irq_enable();
  //toto = 0;

  //RegisterIrqEntry ( 1, &irq_1_handler);
  //mfixed A,B,C,D;
  /*
     int i;
     int  fibov[N], fibot[N];
     fibot[0] = get_cc();
     for (i = 1; i < N; i++)
     {
     fibov[i] = fibo(i);fibot[i] = get_cc();
     }
     fibov [0]= get_cc();

     printf(" Start time %d\n", fibot[0]);

     for (i = 1; i < 10; i++)
     printf(" Fibo %d : %d at %d\n",i,fibov[i],fibot[i]-fibot[i-1]);

     printf("End  time %d\n", fibov[0]);
   */
  // *((volatile unsigned int*) WB_TARGET) = 1;
  // *((volatile unsigned int*) WB_TARGET) = 1;
  // *((volatile unsigned int*) WB_TARGET) = 1;
  // *((volatile unsigned int*) WB_TARGET) = 1;
  // printf("trying to read : \n");
  /*Timer_t* timer = (Timer_t*) WB_TIMER;
  int valeur = timer->Status;
  my_printf("status : %d\n\r",valeur);
  valeur = timer->Control;
  my_printf("control : %d\n\r",valeur);
  valeur = timer->Period;
  my_printf("period : %d\n\r",valeur);
  timer->Snapshot = 0x500;
  valeur = timer->Snapshot;
  my_printf("snapshot : %d\n\r",valeur);
  timer->Period  = 0xfff;
  timer->Control |= 1<<0;
  timer->Control |= 1<<2 ;
  while (1) {
  my_printf("status : %d\n\r",valeur);
  valeur = timer->Control;
  my_printf("control : %d\n\r",valeur);
  valeur = timer->Period;
  my_printf("period : %d\n\r",valeur);
  valeur = timer->Snapshot;
  my_printf("snapshot : %d\n\r",valeur);

    stop(50000);
    if (toto) break;
  } 
  my_printf("and toto is : %d\n\r",toto);
  timer->Period  = 0xfff;
  timer->Control |= 1<<0;
  timer->Control |= 1<<2 ;
  while (1) {
  my_printf("status : %d\n\r",valeur);
  valeur = timer->Control;
  my_printf("control : %d\n\r",valeur);
  valeur = timer->Period;
  my_printf("period : %d\n\r",valeur);
  valeur = timer->Snapshot;
  my_printf("snapshot : %d\n\r",valeur);

    stop(50000);
    if (toto) break;
  } */
  my_printf("and toto is : \n\r");
  while(1){};
  
}
int fibo(int n)
{
  if (n==0)
    return 1;
  else if (n==1)
    return 1;
  else return fibo(n-1) + fibo(n-2);
}
void task1(void *parameters) {
  while(1) {
    my_printf("Task n°:%s \n\r",parameters);
    vTaskDelay(30/portTICK_RATE_MS);
  }
  while(1){};
}
static const char *names[] = { "1","2","3","4"};
int main() {
  int i;
  xTaskHandle my_task;
  xTaskHandle my_task2;
  for (i=0;i<sizeof(names)/sizeof(char *);i++) {
    xTaskCreate(&task1, (const signed char*)"test task", configMINIMAL_STACK_SIZE,names[i],tskIDLE_PRIORITY+1,&my_task);
  }
  vTaskStartScheduler();
  return 0;
}

