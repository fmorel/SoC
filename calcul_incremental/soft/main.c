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
#include "lm32_irq.h"


#define N 10

int a[4][4];

int fibo(int n);

int poly(short X,short Y){
    int X2,Y2;
    X2=X*X;
    Y2=Y*Y;

    return X2*(a[3][0]*X+a[2][1]*Y+a[2][0])+Y2*(a[1][2]*X+a[0][3]*Y+a[0][2])+a[1][1]*X*Y+a[1][0]*X+a[0][1]*Y+a[0][0];

}

int main(void)
{
    int init_time;
    int time[256];
    int result[16][16];
    int i=0;
    int j=0;
    a[3][0]=4;
    a[2][1]=0;
    a[1][2]=0;
    a[0][3]=0;
    a[2][0]=-3*(1<<8);
    a[1][1]=-2*(1<<9);
    a[0][2]=-(1<<8);
    a[1][0]=2*(1<<16);
    a[0][1]=1<<16;
    a[0][0]=0;
    init_time=get_cc();
    for(i=0;i<16;i++){
	for(j=0;j<16;j++){
	    result[i][j] = poly(i+512,j+128);
	    time[i*16+j] = get_cc();
	}
    }
    printf(" Start time : %d\n",time[0]);
    for(i=0;i<16;i++){
	for(j=0;j<16;j++){
	    if(i==0&&j==0)
		printf("Poly %d,%d : %d at %d\n",i+512,j+128,result[i][j]>>16,time[0]-init_time);
	    else
		printf("Poly %d,%d : %d at %d\n",i+512,j+128,result[i][j]>>16,time[i*16+j]-time[i*16+j-1]);
	}
    }
    getchar();
    return 0;
}


int fibo(int n)
{
    if (n==0)
	return 1;
    else if (n==1)
	return 1;
    else return fibo(n-1) + fibo(n-2);
}

