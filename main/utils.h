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
typedef uint32_t image_t[HEIGHT][REAL_WIDTH] ;

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

// tile size
#define TILE_SIZE 16
#define ANTE_TILE_SIZE 32

#endif
