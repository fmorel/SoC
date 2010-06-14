#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
/*********************************************************
******  Definition of all the important parameters *******
*********************************************************/


// Image dimensions
#define WIDTH 640
#define HEIGHT 480
//#define WIDTH 128
//#define HEIGHT 128
#define REAL_WIDTH WIDTH/4

//tile size
#define TILE_SIZE 16
#define ANTE_TILE_SIZE 32
#define NB_TILES (WIDTH*HEIGHT)/(TILE_SIZE*TILE_SIZE)

typedef uint32_t image_t[HEIGHT][REAL_WIDTH] ;

typedef struct incr_coeffs_t {
  uint32_t P0;
  uint32_t Q0;
  uint32_t Q1;
  uint32_t Q2;
  uint32_t Q3;
  uint32_t R0;
  uint32_t R1;
  uint32_t R2;
  uint32_t S0;
  uint32_t S1;
} incr_coeffs_t ;
typedef incr_coeffs_t image_coeffs_t[TILE_SIZE][TILE_SIZE] ;




// Sync
#define TARGET_FPS 			60
#define ENDFRAME_SYNC   40
#define LINE_SYNC       ((int)((25000000/(TARGET_FPS*(HEIGHT+ENDFRAME_SYNC)))-WIDTH))
#define FRAME_SYNC      (WIDTH +LINE_SYNC) * ENDFRAME_SYNC

// Define chunk size of bus access (given in words) 
#define BUS_CHUNK_SIZE 8

// video_out constants
#define VIDEO_OUT_BUFSIZE 2

// video_in
#define VIDEO_IN_BUFSIZE 4
#endif
