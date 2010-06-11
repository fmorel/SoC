/*********************************************************
******  Definition of all the important parameters *******
*********************************************************/


//size of image
#define WIDTH 640
#define HEIGHT 480

//video_out constants
#define VIDEO_OUT_BUFSIZE 4

//video_in
#define VIDEO_IN_BUFSIZE 4

//tile size
#define TILE_SIZE 16
#define ANTE_TILE_SIZE 32
#define NB_TILES (WIDTH*HEIGHT)/(TILE_SIZE*TILE_SIZE)
