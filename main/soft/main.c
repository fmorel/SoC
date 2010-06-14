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



const static int a[4][4] =
{ { 0  ,0  ,0  ,0  ,} ,
  { 1,0  ,0  ,0 } ,
  { 0  ,0  ,0  ,0 } ,
  { 0  ,0  ,0  ,0 } } ;

const static int b[4][4] =
{ { 0  ,0  ,0  ,0  ,} ,
  { 0  ,0  ,0  ,0 } ,
  { 0  ,1 ,0  ,0 } ,
  { 0  ,0  ,0  ,0 } } ;


static int dataPoly[NB_TILES*20];

void computePolys (void) {
  // irq_enable();
  //RegisterIrqEntry(3, &new_tile_handler);

  const int a_c10 = 6*a[3][0];
  const int a_c01 = 2 * a[2][1];
  const int a_c00 = 6 * a[3][0] + 2 * a[2][0];
  const int a_b20 = 3*a[3][0];
  const int a_b11 = 2*a[2][1];
  const int a_b02 = a[1][2];
  const int a_b10 = 3 * a[3][0] + 2 * a[2][0];
  const int a_b01 = a[2][1] + a[1][1];
  const int a_b00 = a[3][0] + a[2][0] + a[1][0];
  const int b_c10 = 6*b[3][0];
  const int b_c01 = 2 * b[2][1];
  const int b_c00 = 6 * b[3][0] + 2 * b[2][0];
  const int b_b20 = 3*b[3][0];
  const int b_b11 = 2*b[2][1];
  const int b_b02 = b[1][2];
  const int b_b10 = 3 * b[3][0] + 2 * b[2][0];
  const int b_b01 = b[2][1] + b[1][1];
  const int b_b00 = b[3][0] + b[2][0] + b[1][0];
  int time[11];
  int i;
  int line,col;
  int X=0;
  int Y=0;
  int X_2;
  int Y_2;
  int a_P1,a_P2,a_P3,a_P0,a_Q0,a_Q1,a_Q2,a_R0,a_R1,a_R2,a_S0,a_S1;
  int b_P1,b_P2,b_P3,b_P0,b_Q0,b_Q1,b_Q2,b_R0,b_R1,b_R2,b_S0,b_S1;



  line=0;col=0;
  my_printf(" Processoooorr begin to compuuuute !\r\n");
  for(i=0;i<NB_TILES;i++) {
    X=16*col;
    X_2 = X*X;
    Y=16*line;
    Y_2 = Y*Y;
    col++;
    if (col == WIDTH/TILE_SIZE) {
      line++;
      col=0;
      my_printf("Line ended !! \r\n");
    }
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




    dataPoly[i*20+0] = a_Q0;
    dataPoly[i*20+1] = a_Q1;
    dataPoly[i*20+2] = a_Q2;
    dataPoly[i*20+3] = a_P3;
    dataPoly[i*20+4] = a_R0;
    dataPoly[i*20+5] = a_R1;
    dataPoly[i*20+6] = a_R2;
    dataPoly[i*20+7] = a_S0;
    dataPoly[i*20+8] = a_S1;
    dataPoly[i*20+9] = a_P0;

    dataPoly[i*20+10] = b_Q0;
    dataPoly[i*20+11] = b_Q1;
    dataPoly[i*20+12] = b_Q2;
    dataPoly[i*20+13] = b_P3;
    dataPoly[i*20+14] = b_R0;
    dataPoly[i*20+15] = b_R1;
    dataPoly[i*20+16] = b_R2;
    dataPoly[i*20+17] = b_S0;
    dataPoly[i*20+18] = b_S1;
    dataPoly[i*20+19] = b_P0;

  }
}





#define INCREMENT *((volatile unsigned int *) INCREMENT_BASE )
#define OUTPUT   *((volatile unsigned int *) OUTPUT_BASE)
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
    

    computePolys();
    irq_enable();

    RegisterIrqEntry(2, &video_in_handler);

    //Initialize processing
    processing_index = 0;

    //Initialize VIDEO_IN
    video_in_index = 0;
    VIDEO_IN  = (uint32_t)&images[0] ;

    //Initialize VIDEO_OUT
    VIDEO_OUT = (uint32_t)&images[IMAGES_NUMBER-1];

    video_processing_task();

    getchar();
    return 0;
  }



