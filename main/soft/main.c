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
#include "lm32_irq.h"
#include "segmentation.h"

#define VIDEO_OUT   *((volatile unsigned int *)VIDEO_OUT_BASE)
#define VIDEO_IN    *((volatile unsigned int *)VIDEO_IN_BASE)

//in word (4 bytes)
#define WIDTH 160
#define HEIGHT 480
#define MODULO 16

uint32_t image[HEIGHT][WIDTH] = { { 0 } };
uint32_t image2[HEIGHT][WIDTH] = { { 0 } };
volatile int img_n = 0;
volatile int lock_video_in = 0;
volatile int lock_video_out = 0;

int damier(int i, int j) {
	i = i % MODULO;
	j *= 4;
	j = j % MODULO;
	if (i<MODULO/2 && j<MODULO/2) return 0xffffffff;
	if (i>= MODULO/2 && j< MODULO/2) return 0;
	if (i< MODULO/2 && j>= MODULO/2) return 0;
	return 0xffffffff;
}

void video_out_handler() {
    lock_video_out = 1;
    my_printf("VIDEO_OUT: interrupt\n\r");
}

void video_in_handler() {
//    my_printf("VIDEO_IN: interrupt\n\r");
		lock_video_in = 1;
}

int main(void) {

    irq_enable();

    RegisterIrqEntry(1, &video_out_handler);
    RegisterIrqEntry(2, &video_in_handler);

    int i,j;

//	for (i=0;i<HEIGHT;i++) {
//		for (j=0;j<WIDTH/4;j++) {
//			image[i][j]=damier(i,j);
//		}
//	}
	
//    lock_video_out = 1;
//    for(i = 0; i < 5; i++) {
//        while(!lock_video_out);
//        my_printf("Sending %i\n\r", i);
//        lock_video_out = 0;
//        VIDEO_OUT = (uint32_t)image;
//    }
//    my_printf("Done\n");


    int toto_in = 1;
		int toto_out= 0;

    lock_video_in = 1;
    lock_video_out = 1;
    for (i=0; i < 20; i++) {
        while(!lock_video_in && !lock_video_out);
        if(lock_video_in) {
            toto_in = !toto_in;
            lock_video_in = 0;
            VIDEO_IN = toto_in ? (uint32_t)image : (uint32_t)image2;
        }
        if(lock_video_out) {
						toto_out =!toto_out;
            lock_video_out = 0;
            VIDEO_OUT = toto_out ? (uint32_t)image : (uint32_t)image2;
        }
    }

//        VIDEO_IN = (uint32_t)image;
//        while(!lock_video_in);
//        VIDEO_OUT = (uint32_t)image;

//    // Test video_in
//    for(i = 0; i < 5; i++) {
//        VIDEO_IN = (i % 2) == 0 ? (uint32_t)image : (uint32_t)image2;
//        while(!lock_video_out);
//        VIDEO_OUT = (i % 2) == 0 ? (uint32_t)image : (uint32_t)image2;
//        lock = 0;
//
//    }

    getchar();
    return 0;
}

