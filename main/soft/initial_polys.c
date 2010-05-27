#include "initial_polys.h"
#include "my_printf.h"
#include "lm32_irq.h"
static int a[4][4] =
{ { 1,1,1,1,} ,
  { 1,1,1,1 } ,
  { 1,1,1,1 } ,
  { 1,1,1,1 } } ;

void print_a() {
  int i,j;
  for (i = 0;i<4;i++) {
    for (j=0;j<4;j++)
    {
      my_printf("%d ",a[i][j]);
    }
    my_printf("\n\r");
  }
}

void calc_polys() {
  int begin_cycles = get_cc();
  int X=0,Y=0;

 

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
  float P2 = b20 * X * X + b11 * X * Y + b02 * Y * Y +\
           b10 * X + b01 * Y +\
           b00 ;
  //P3(X,Y) and Q3(X0,0)
  float P3 = a[3][0] * X * X * X + a[2][1] * X * X * Y + a[1][2] * X * Y * Y + a[0][3] * Y * Y * Y +\
           a[2][0] * X * X + a[1][1] * X * Y + a[0][2] * Y * Y +\
           a[1][0] * X + a[0][1] * Y +\
           a[0][0] ;


  float P0 = 6 * a[3][0];
  
  float Q0 = 6 * a[0][3];
  float Q1 = 6 * a[0][3] + 2 * a[0][2] +2 * a[1][2] * X + 6 * a[0][3] * Y ;
  float Q2 = a[0][3] + a[0][2]+ a[0][1] + a[2][1] * X * X + 2 * a[1][2] * X * Y \
           + 3* a[0][3] * Y * Y + (a[1][2] + a[1][1] ) * X + ( 3 * a[0][3] + 2 * a[0][2] ) * Y ;
  float Q3 = P3 ;

  float R0 = 2* a[1][2] ;
  float R1 = a[1][2] + a[2][1] + a[1][1] + 2 * a[1][2] * X + 2 * a[1][2] * Y ;
  float R2 = P2;

  float S0 = 2* a[2][1] ;
  float S1 = P1;

  int cycles = get_cc();
  my_printf("%x ",P0);
  my_printf("%x ",Q0);
  my_printf("%x ",Q1);
  my_printf("%x ",Q2);
  my_printf("%x ",Q3);
  my_printf("%x ",R0);
  my_printf("%x ",R1);
  my_printf("%x ",R2);
  my_printf("%x ",S0);
  my_printf("%x ",S1);
  my_printf("\n\r");
  my_printf("Time : %d\n\r",cycles - begin_cycles);


}
