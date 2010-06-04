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

static float a[4][4] =
   { { 0,0,0,0,} ,
     { 0.5,0,0,0 } ,
     { 0,0,0,0 } ,
     { 0,0,0,0 } } ;


int main(void) {
    int i;
    int X=16;
    int Y=16;
        //P1(X,Y) and S1(X0,Y0)
    float c10 = 6*a[3][0];
    float c01 = 2 * a[2][1];
    float c00 = 6 * a[3][0] + 2 * a[2][0];
    float P1 = c10 * X + c01 * Y + c00;
    //P2(X,Y) and R2(X0,Y0)
    float b20 = 3*a[3][0];
    float b11 = 2*a[2][1];
    float b02 = a[1][2];
    float b10 = 3 * a[3][0] + 2 * a[2][0];
    float b01 = a[2][1] + a[1][1];
    float b00 = a[3][0] + a[2][0] + a[1][0];
    float P2 = b20 * X * X + b11 * X * Y + b02 * Y * Y +
	       b10 * X + b01 * Y +
	       b00 ;
    //P3(X,Y) and Q3(X0,0)
    float P3 = a[3][0] * X * X * X + a[2][1] * X * X * Y + a[1][2] * X * Y * Y + a[0][3] * Y * Y * Y +
	       a[2][0] * X * X + a[1][1] * X * Y + a[0][2] * Y * Y +
	       a[1][0] * X + a[0][1] * Y +
	       a[0][0] ;


    float P0 = 6 * a[3][0];

    float Q0 = 6 * a[0][3];
    float Q1 = 6 * a[0][3] + 2 * a[0][2] +2 * a[1][2] * X + 6 * a[0][3] * Y ;
    float Q2 = a[0][3] + a[0][2]+ a[0][1] + a[2][1] * X * X + 2 * a[1][2] * X * Y 
	       + 3* a[0][3] * Y * Y + (a[1][2] + a[1][1] ) * X + ( 3 * a[0][3] + 2 * a[0][2] ) * Y ;
    float Q3 = P3 ;

    float R0 = 2* a[1][2] ;
    float R1 = a[1][2] + a[2][1] + a[1][1] + 2 * a[1][2] * X + 2 * a[1][2] * Y ;
    float R2 = P2;

    float S0 = 2* a[2][1] ;
    float S1 = P1;

    for(i=0;i<5;i++) {
	*((volatile unsigned int*) 0xB0000000) = P0;
	*((volatile unsigned int*) 0xB0000000) = P1;
	*((volatile unsigned int*) 0xB0000000) = P2;
	*((volatile unsigned int*) 0xB0000000) = Q3;
	*((volatile unsigned int*) 0xB0000000) = Q0;
	*((volatile unsigned int*) 0xB0000000) = Q1;
	*((volatile unsigned int*) 0xB0000000) = Q2;
	*((volatile unsigned int*) 0xB0000000) = R0;
	*((volatile unsigned int*) 0xB0000000) = R1;
	*((volatile unsigned int*) 0xB0000000) = R2;
	*((volatile unsigned int*) 0xB0000000) = S0;
	*((volatile unsigned int*) 0xB0000000) = S1;
    }



    getchar();
    return 0;

}
