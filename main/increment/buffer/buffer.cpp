#include "buffer.h"
#include <iostream>

namespace soclib { namespace caba {
	// Constructor
		Buffer::Buffer(sc_core::sc_module_name insname)
		: sc_core::sc_module(insname)
		{
			SC_METHOD(bufferTransition);
			dont_initialize();
			sensitive<<p_clk.pos();
			SC_METHOD(bufferMoore);
			dont_initialize();
			sensitive<<p_clk.neg();
		}

	void Buffer::bufferTransition() {
    if (buffer_write_enable) {
      uint32_t temp = buffer_in.read();
      buf[1-read_buffer][counter/ANTE_TILE_SIZE][counter%ANTE_TILE_SIZE]         = temp >> 24 & 0xff;
      buf[1-read_buffer][(counter+1)/ANTE_TILE_SIZE][(counter+1)%ANTE_TILE_SIZE] = temp>>16   & 0xff;
      buf[1-read_buffer][(counter+2)/ANTE_TILE_SIZE][(counter+2)%ANTE_TILE_SIZE] = temp>>8    & 0xff;
      buf[1-read_buffer][(counter+3)/ANTE_TILE_SIZE][(counter+3)%ANTE_TILE_SIZE] = temp       & 0xff;
    }
  }

	void Buffer::bufferMealy() {
    int x = buffer_command_out[0];
    int y = buffer_command_out[1];
    buffer_out[0] = buf[read_buffer][x][y];
    buffer_out[1] = buf[read_buffer][x+1][y];
    buffer_out[2] = buf[read_buffer][x][y+1];
    buffer_out[3] = buf[read_buffer][x+1][y+1];


    if (buffer_write_enable) {
      counter++;
    }
    if (counter >= ANTE_TILE_SIZE * ANTE_TILE_SIZE) {
      counter = 0;
      read_buffer = 1 - read_buffer;
    } 
  }
}}


