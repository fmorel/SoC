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
#include "my_printf.h"

#define VIDEO_OUT *((volatile unsigned int *)VIDEO_OUT_BASE)

//in word (4 bytes)
#define WIDTH 160
#define HEIGHT 480
#define MODULO 16

const static float a[4][4] =
   { { 1  ,1  ,1  ,1  ,} ,
     { 1.5,1  ,1  ,1 } ,
     { 1  ,1  ,1  ,1 } ,
     { 1  ,1  ,1  ,1 } } ;

const static float b[4][4] =
   { { 1  ,1  ,1  ,1  ,} ,
     { 1  ,1  ,1  ,1 } ,
     { 1  ,1.5,1  ,1 } ,
     { 1  ,1  ,1  ,1 } } ;

int main(void) {
   const float a_c10 = 6*a[3][0];
    const float a_c01 = 2 * a[2][1];
    const float a_c00 = 6 * a[3][0] + 2 * a[2][0];
    const float a_b20 = 3*a[3][0];
    const float a_b11 = 2*a[2][1];
    const float a_b02 = a[1][2];
    const float a_b10 = 3 * a[3][0] + 2 * a[2][0];
    const float a_b01 = a[2][1] + a[1][1];
    const float a_b00 = a[3][0] + a[2][0] + a[1][0];
    const float b_c10 = 6*b[3][0];
    const float b_c01 = 2 * b[2][1];
    const float b_c00 = 6 * b[3][0] + 2 * b[2][0];
    const float b_b20 = 3*b[3][0];
    const float b_b11 = 2*b[2][1];
    const float b_b02 = b[1][2];
    const float b_b10 = 3 * b[3][0] + 2 * b[2][0];
    const float b_b01 = b[2][1] + b[1][1];
    const float b_b00 = b[3][0] + b[2][0] + b[1][0];
    int time[7];
    int i;
    int X=0;
    int Y=0;
    int X_2;
    int Y_2;
    float a_P1,a_P2,a_P3,a_P0,a_Q0,a_Q1,a_Q2,a_R0,a_R1,a_R2,a_S0,a_S1;
    float b_P1,b_P2,b_P3,b_P0,b_Q0,b_Q1,b_Q2,b_R0,b_R1,b_R2,b_S0,b_S1;
    time[6]=get_cc();
    for(i=0;i<6;i++) {
	X=16*i;
	X_2 = X*X;
	Y=0;
	Y_2 = Y*Y;
	//P1(X,Y) and S1(X0,Y0)
	a_P1 = a_c10 * X + a_c01 * Y + a_c00;
	//P2(X,Y) and R2(X0,Y0)
	a_P2 = a_b20 * X * X + a_b11 * X * Y + a_b02 * Y * Y +
	    a_b10 * X + a_b01 * Y +
	    a_b00 ;
	//P3(X,Y) and Q3(X0,0)
	a_P3 = X_2*(a[3][0] * X + a[2][1] * Y + a[2][0]) + Y_2*(a[1][2] * X + a[0][3] * Y + a[0][2]) +
	    a[1][1] * X * Y +
	    a[1][0] * X + a[0][1] * Y +
	    a[0][0] ;


	a_P0 = 6 * a[3][0];

	a_Q0 = 6 * a[0][3];
	a_Q1 = 6 * a[0][3] + 2 * a[0][2] +2 * a[1][2] * X + 6 * a[0][3] * Y ;
	a_Q2 = a[0][3] + a[0][2]+ a[0][1] + a[2][1] * X_2 + 2 * a[1][2] * X * Y 
	    + 3* a[0][3] * Y_2 + (a[1][2] + a[1][1] ) * X + ( 3 * a[0][3] + 2 * a[0][2] ) * Y ;

	a_R0 = 2* a[1][2] ;
	a_R1 = a[1][2] + a[2][1] + a[1][1] + 2 * a[1][2] * X + 2 * a[1][2] * Y ;
	a_R2 = a_P2;

	a_S0 = 2* a[2][1] ;
	a_S1 = a_P1;

	////////// FOR Y INCR.

	//P1(X,Y) bnd S1(X0,Y0)
	b_P1 = b_c10 * X + b_c01 * Y + b_c00;
	//P2(X,Y) bnd R2(X0,Y0)
	b_P2 = b_b20 * X * X + b_b11 * X * Y + b_b02 * Y * Y +
	    b_b10 * X + b_b01 * Y +
	    b_b00 ;
	//P3(X,Y) bnd Q3(X0,0)
	b_P3 = X_2*(b[3][0] * X + b[2][1] * Y + b[2][0]) + Y_2*(b[1][2] * X + b[0][3] * Y + b[0][2]) +
	    b[1][1] * X * Y +
	    b[1][0] * X + b[0][1] * Y +
	    b[0][0] ;


	b_P0 = 6 * b[3][0];

	b_Q0 = 6 * b[0][3];
	b_Q1 = 6 * b[0][3] + 2 * b[0][2] +2 * b[1][2] * X + 6 * b[0][3] * Y ;
	b_Q2 = b[0][3] + b[0][2]+ b[0][1] + b[2][1] * X_2 + 2 * b[1][2] * X * Y 
	    + 3* b[0][3] * Y_2 + (b[1][2] + b[1][1] ) * X + ( 3 * b[0][3] + 2 * b[0][2] ) * Y ;

	b_R0 = 2* b[1][2] ;
	b_R1 = b[1][2] + b[2][1] + b[1][1] + 2 * b[1][2] * X + 2 * b[1][2] * Y ;
	b_R2 = b_P2;

	b_S0 = 2* b[2][1] ;
	b_S1 = b_P1;

	/////////// SEND
	*((volatile unsigned int*) 0xB0000000) = a_P0;
	*((volatile unsigned int*) 0xB0000004) = a_P1;
	*((volatile unsigned int*) 0xB0000008) = a_P2;
	*((volatile unsigned int*) 0xB000000C) = a_P3;
	*((volatile unsigned int*) 0xB0000010) = a_Q0;
	*((volatile unsigned int*) 0xB0000014) = a_Q1;
	*((volatile unsigned int*) 0xB0000018) = a_Q2;
	*((volatile unsigned int*) 0xB000001C) = a_R0;
	*((volatile unsigned int*) 0xB0000020) = a_R1;
	*((volatile unsigned int*) 0xB0000024) = a_R2;
	*((volatile unsigned int*) 0xB0000028) = a_S0;
	*((volatile unsigned int*) 0xB000002C) = a_S1;


	*((volatile unsigned int*) 0xB0000000) = b_P0;
	*((volatile unsigned int*) 0xB0000004) = b_P1;
	*((volatile unsigned int*) 0xB0000008) = b_P2;
	*((volatile unsigned int*) 0xB000000C) = b_P3;
	*((volatile unsigned int*) 0xB0000010) = b_Q0;
	*((volatile unsigned int*) 0xB0000014) = b_Q1;
	*((volatile unsigned int*) 0xB0000018) = b_Q2;
	*((volatile unsigned int*) 0xB000001C) = b_R0;
	*((volatile unsigned int*) 0xB0000020) = b_R1;
	*((volatile unsigned int*) 0xB0000024) = b_R2;
	*((volatile unsigned int*) 0xB0000028) = b_S0;
	*((volatile unsigned int*) 0xB000002C) = b_S1;
	time[5-i]=get_cc();
    }
    for(i=0;i<6;i++){
	my_printf("TIME : %d\r\n", (time[i]-time[i+1]));
    }
    getchar();
    return 0;

}



