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
#include "utils.h"
#include "process.h"
#include "lm32_irq.h"
#include "segmentation.h"
#include "my_printf.h"

#define VIDEO_OUT   *((volatile unsigned int *)VIDEO_OUT_BASE)
#define VIDEO_IN    *((volatile unsigned int *)VIDEO_IN_BASE)

#define IMAGES_NUMBER 8

volatile unsigned int video_in_index;
volatile unsigned int processing_index;
volatile int difference;
image_t images[IMAGES_NUMBER] = { { { 0 } } };
image_coeffs_t image_coeffs;


void video_in_handler() {
  video_in_index++;
  //Give video_in an address.
  VIDEO_IN  = (uint32_t)&images[(video_in_index)%IMAGES_NUMBER] ;
  if (video_in_index - processing_index == 2) {
    disable_irq (2);
  }
}

void video_processing_task() {
  for(;;) {
    //Wait unilt video_in_index is greater than processing_index
    while(video_in_index  == processing_index);

    uint32_t image_address = (uint32_t)&images[processing_index%IMAGES_NUMBER];
    my_printf("Processing image : %x\n\r",image_address);
    vProcessImage(*((image_t *)image_address));
    VIDEO_OUT = image_address;
    my_printf("Processed image : %x\n\r",image_address);
    processing_index++;
    //Enable irq because we finished processing one image.
    enable_irq(2);
  }
}

int main(void) {

  irq_enable();

  RegisterIrqEntry(2, &video_in_handler);

  //Initialize processing
  processing_index = 0;

  //Initialize VIDEO_IN
  video_in_index = 0;
  VIDEO_IN  = (uint32_t)&images[0] ;

  video_processing_task();

  getchar();
  return 0;
}

