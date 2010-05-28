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


static int a[4][4];
static int b[4][4];

int poly(short X,short Y,short reg){
    int X2,Y2;
    X2=X*X;
    Y2=Y*Y;
    int result;
    switch(reg) {
	case 0 : result = b[2][0]<<1; break;
	case 1 : result = (b[2][0]<<1) * X + (a[2][1]<<1) * Y + b[2][0] + b[1][0]; break;
	case 2 : result = b[2][0]*X2+b[1][1]*X*Y+b[0][2]*Y2+b[1][0]*X+b[0][1]*Y+b[0][0]; break;
	case 3 : result = X2*(a[3][0]*X+a[2][1]*Y+a[2][0])+Y2*(a[1][2]*X+a[0][3]*Y+a[0][2])+a[1][1]*X*Y+a[1][0]*X+a[0][1]*Y+a[0][0]; break;
	case 4 : result = 6*a[0][3]; break;
	case 5 : result = 6*a[0][3]; break;
	case 6 : result = (a[1][2]<<1)*X + 6*a[0][3]*Y + 6*a[0][3]+a[0][2]; break;
	case 7 : result = a[2][1]*X2+(a[1][2]<<1)*X*Y+3*a[0][3]*Y2+(a[1][2]+a[1][1])*X+(3*a[0][3]+2*a[0][2])*Y+a[0][3]+a[0][2]+a[0][1];break;
	case 8 : result = 2*a[1][2]; break;
	case 9 : result = 2*a[1][2]; break;
	case 10 : result = 2*a[1][2]*(X+Y)+a[1][2]+a[2][1]+a[1][1];break;
	case 11 : result = 2*a[2][1];break;
	case 12 : result = 2*a[2][1];break;
	default : result = 0; break;
    }
    return result;
}

int main(void)
{
    int i = 0;
    //    int i = 0;
    int j = 0;
    int k = 0;
    a[3][0]=0;
    a[2][1]=0;
    a[1][2]=0;
    a[0][3]=0;
    a[2][0]=0;
    a[1][1]=0;
    a[0][2]=0;
    a[1][0]=1;
    a[0][1]=0;
    a[0][0]=0;

    b[3][0]=0;
    b[2][1]=0;
    b[1][2]=0;
    b[0][3]=0;
    b[2][0]=0;
    b[1][1]=0;
    b[0][2]=0;
    b[1][0]=0;
    b[0][1]=1;
    b[0][0]=0;
    
//    a[3][0]=4;
//    a[2][1]=1;
//    a[1][2]=1;
//    a[0][3]=1;
//    a[2][0]=-3*(1<<8);
//    a[1][1]=-2*(1<<9);
//    a[0][2]=-(1<<8);
//    a[1][0]=2*(1<<16);
//    a[0][1]=1<<16;
//    a[0][0]=1;
//
//    b[3][0]=1;
//    b[2][1]=4;
//    b[1][2]=1;
//    b[0][3]=4;
//    b[2][0]=-(1<<8);
//    b[1][1]=-2*(1<<9);
//    b[0][2]=-3*(1<<8);
//    b[1][0]=(1<<16);
//    b[0][1]=2*(1<<16);
//    b[0][0]=1;
    for(j= 0 ; j<40 ; j++){
	for(k=0 ; k<30 ; k++){
	    for(i = 0; i<13; i++){
		*((volatile unsigned int*) 0xB0000000) = poly(j*16,k*16,i)>>1;
	    }
	    i = 0;
	    while( i < 500)
	    {
		i++;
	    }

	}
    }
    for(i = 0; i < 13; i++){
	j+=4;
    }
    /*
       printf(" start time : %d & end time : %d\n",init_time,end_time);
       printf(" average duration : %d \n",(end_time-init_time));
       for(i=0;i<13;i++){
       printf(" result : %d\n",poly_to_send[i]);
       }

       getchar();*/
    return 0;
}

