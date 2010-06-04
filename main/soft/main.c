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

#define VIDEO_OUT *((volatile unsigned int *)VIDEO_OUT_BASE)

//in word (4 bytes)
#define WIDTH 160
#define HEIGHT 480
#define MODULO 16

int damier(int i, int j) {
	i = i % MODULO;
	j *= 4;
	j = j % MODULO;
	if (i<MODULO/2 && j<MODULO/2) return 0xffffffff;
	if (i>= MODULO/2 && j< MODULO/2) return 0;
	if (i< MODULO/2 && j>= MODULO/2) return 0;
	return 0xffffffff;
}

#include "initial_polys.h"
int main(void)
{
    int i,j;
//		static uint32_t image[HEIGHT][WIDTH] = { { 0 } };
/*
		for (i=0;i<HEIGHT;i++) {
			for (j=0;j<WIDTH/4;j++) {
				image[i][j]=damier(i,j);
			}
		}
	
   */
	//printf("address :0x%x \n",(uint32_t)image);
  //		VIDEO_OUT=(uint32_t) image;
//    print_a();
//    calc_polys();


    getchar();
    return 0;
}

