/*********************************************************
******  Definition of all the important parameters *******
*********************************************************/


// Image dimensions
#define WIDTH 640
#define HEIGHT 480
//#define WIDTH 128
//#define HEIGHT 128

// Sync
#define LINE_SYNC       ((int)((25000000/60-HEIGHT*WIDTH)/HEIGHT))
#define ENDFRAME_SYNC   40
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
