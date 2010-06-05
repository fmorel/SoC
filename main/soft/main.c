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
#include "my_printf.h"
#include "utils.h"

//memcpy
#include <string.h>

#define VIDEO_OUT   *((volatile unsigned int *)VIDEO_OUT_BASE)
#define VIDEO_IN    *((volatile unsigned int *)VIDEO_IN_BASE)


#define IMAGES_NUMBER 5

#define REAL_WIDTH WIDTH/4
typedef uint32_t image_t[HEIGHT][REAL_WIDTH] ;

int ante_X(int i,int j) {
  return i/2 + HEIGHT / 4;
}
int ante_Y(int i,int j) {
  return j/2 + REAL_WIDTH / 4;
}
void vZoom(image_t image) {
  static image_t image_copy; 
  int i,j;
  for (i=0;i<HEIGHT;i++) {
    for (j=0;j<REAL_WIDTH;j++) {
      image_copy[i][j] = image[i][j];
    }
  }
  for (i = HEIGHT-1;i>=1;i-=2) {
    for(j=REAL_WIDTH-1;j>=0;j--) {
      int X = ante_X(i,j);
      int Y = ante_Y(i,j);
      uint32_t pixels = image_copy[X][Y]; 
      uint32_t results=0;
      uint8_t pixel1;
      uint8_t pixel2;
      if (j%2==0) {
        pixel1 = pixels>>24;
        pixel2 = pixels>>16;
      } else {
        pixel1 = pixels>>8;
        pixel2 = pixels>>0;

      }
      results = (pixel1<<24) | (pixel1<<16) | (pixel2<<8) | (pixel2<<0); 
      image[i-1][j] = results;
      image[i][j] = results;
    }
  }
  //Processing takes a long time !
 // vTaskDelay(1000);
}
void vProcessImage(image_t image) {
  vZoom(image);
}


xSemaphoreHandle xSemaphore_video_in;
xSemaphoreHandle xSemaphore_video_out;
xQueueHandle xQueue_valid_images_address;
xQueueHandle xQueue_processed_images_address;
void video_out_handler() {
  /* Unblock the task by releasing the semaphore. */
  xSemaphoreGiveFromISR( xSemaphore_video_out, pdFALSE );
}

void video_in_handler() {
  /* Unblock the task by releasing the semaphore. */
  xSemaphoreGiveFromISR( xSemaphore_video_in, pdFALSE );
}
void video_in_task(void *parameters) {
  int i;
  static image_t images[IMAGES_NUMBER] = { { { 0 } } };

  for (i=0;;i++) {
    image_t *current_image = &images[i%IMAGES_NUMBER] ;
    //Give video_in an address.
    VIDEO_IN  = (uint32_t)current_image;
    //Wait for video_in to complete transfer
    xSemaphoreTake( xSemaphore_video_in, portMAX_DELAY );


    //Post the new valid image to the queue.
    //Drop it if the queue is full
    if (pdTRUE != xQueueSend(xQueue_valid_images_address,&current_image, 0 )) {
      //We could not post to the queue.
      //Overwrite previous image because processing didn't get it.
      i--;
      my_printf("Queue is FULL, dropping Frame..\n\r");
    }
  }
}
void video_processing_task(void *parameters) {
  image_t *image_address;
  for(;;) {
    xQueueReceive(xQueue_valid_images_address,&image_address, portMAX_DELAY);
    vProcessImage(*image_address);
    xQueueSend(xQueue_processed_images_address,&image_address,0);
  } 
}
void video_out_task(void *parameters) {
  static image_t dummy_image = { { 0 } };
  image_t* image_address = &dummy_image;
  image_t* previous_image_address;
  for(;;) {

    //Get the image address from the queue. Do not wait.
    previous_image_address = image_address;
    if (pdTRUE != xQueueReceive(xQueue_processed_images_address,&image_address, 0)) {
      //We didn't get a new image. Use the previous one.
      image_address = previous_image_address;
    } 
    //Tell video_out
    VIDEO_OUT = (uint32_t)image_address;

    //Wait for Video_out to ask for an image.
    xSemaphoreTake( xSemaphore_video_out, portMAX_DELAY );
  }

}

int main(void) {
  //Create two taken Semaphores
  xSemaphore_video_in  = xQueueCreate( ( unsigned portBASE_TYPE ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH );	
  xSemaphore_video_out = xQueueCreate( ( unsigned portBASE_TYPE ) 1, semSEMAPHORE_QUEUE_ITEM_LENGTH );	
  //Create the images address queue
  xQueue_valid_images_address     = xQueueCreate( ( unsigned portBASE_TYPE ) 1 , sizeof(uint32_t)); 
  xQueue_processed_images_address = xQueueCreate( ( unsigned portBASE_TYPE ) 1 , sizeof(uint32_t)); 

  //Register interrupt handlers
  RegisterISR(1,NULL, &video_out_handler);
  RegisterISR(2,NULL, &video_in_handler);

  //Check that the semaphore was created.
  if (xSemaphore_video_in == NULL || xSemaphore_video_out == NULL) {
    getchar();
    return 0;
  }

  xTaskCreate(&video_in_task        , (const signed char*)"video_in task", configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+2,NULL);
  xTaskCreate(&video_out_task       , (const signed char*)"video_out task", configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+3,NULL);
  xTaskCreate(&video_processing_task, (const signed char*)"video_out task", configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY+1,NULL);
  vTaskStartScheduler();
  return 0;
}

