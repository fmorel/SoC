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
#include "../segmentation.h"

#define VIDEO_OUT *((volatile unsigned int *)VIDEO_OUT_BASE)

//in word (4 bytes)
#define WIDTH 640
#define HEIGHT 480
#define MODULO 10

int damier(int i, int j) {
	i = i % MODULO;
	j = j % MODULO;
	if (i<MODULO/2 && j<MODULO/2) return 0xffffffff;
	if (i>= MODULO/2 && j< MODULO/2) return 0;
	if (i< MODULO/2 && j>= MODULO/2) return 0;
	return 0xffffffff;
}

int main(void)
{
    //mfixed A,B,C,D;
    int i,j;
		static unsigned char image[HEIGHT][WIDTH] = { { 0 } };

		printf("%x %x %x %x\n",&image[0][0],&image[0][1],&image[0][2],&image[0][3]);
		printf("toto \n");
		printf("%x %x %x %x\n",&image[1][0],&image[1][1],&image[1][2],&image[1][3]);

			for (j=0;j<WIDTH;j++) {
				image[0][j] = 0xff;
			}
		printf("%x %x %x %x\n",image[0][0],image[0][1],image[0][2],image[0][3]);
		printf("toto \n");
		printf("%x %x %x %x\n",image[1][0],image[1][1],image[1][2],image[1][3]);

		for (i=0;i<HEIGHT/4;i++) {
			for (j=0;j<WIDTH/4;j++) {
				image[i][j]=damier(i,j);
			}
		}
	
	printf("address :0x%x \n",(uint32_t)image);
		VIDEO_OUT=(uint32_t) image;


    getchar();
    return 0;
}


