#include "process.h"


int ante_X(int i,int j) {
  return i/2 + HEIGHT / 4;
}
int ante_Y(int i,int j) {
  return j/2 + REAL_WIDTH / 4;
}
void vZoom(image_t image) {
  static image_t image_copy;
  int i,j;
  for (i=0;i<HEIGHT;i++) {
    for (j=0;j<REAL_WIDTH;j++) {
      image_copy[i][j] = image[i][j];
    }
  }
  for (i = HEIGHT-1;i>=1;i-=2) {
    for(j=REAL_WIDTH-1;j>=0;j--) {
      int X = ante_X(i,j);
      int Y = ante_Y(i,j);
      uint32_t pixels = image_copy[X][Y];
      uint32_t results=0;
      uint8_t pixel1;
      uint8_t pixel2;
      if (j%2==0) {
        pixel1 = pixels>>24;
        pixel2 = pixels>>16;
      } else {
        pixel1 = pixels>>8;
        pixel2 = pixels>>0;

      }
      results = (pixel1<<24) | (pixel1<<16) | (pixel2<<8) | (pixel2<<0);
      image[i-1][j] = results;
      image[i][j] = results;
    }
  }
}
void vProcessImage(image_t image) {
  vZoom(image);
}


