#include "single_buffer.h"
#include <iostream>

namespace soclib { namespace caba {
  // Constructor
  SingleBuffer::SingleBuffer(sc_core::sc_module_name insname)
    : sc_core::sc_module(insname)
  {
    SC_METHOD(transition);
    dont_initialize();
    sensitive<<p_clk.pos();
    SC_METHOD(genMoore);
    dont_initialize();
    sensitive<<p_clk.neg();
  }

  void SingleBuffer::transition() {

    if (!p_resetn) {
      counter = command_out_x = command_out_y = 0;
      return;
    } 

    command_out_x = buffer_command_out[0];
    command_out_y = buffer_command_out[1];

    if (buffer_write_enable) {
      uint32_t temp = buffer_in.read();
      buf[(counter/ANTE_TILE_SIZE)%ANTE_TILE_SIZE][counter%ANTE_TILE_SIZE]         = temp >> 24 & 0xff;
      buf[((counter+1)/ANTE_TILE_SIZE%ANTE_TILE_SIZE)][(counter+1)%ANTE_TILE_SIZE] = temp>>16   & 0xff;
      buf[((counter+2)/ANTE_TILE_SIZE%ANTE_TILE_SIZE)][(counter+2)%ANTE_TILE_SIZE] = temp>>8    & 0xff;
      buf[((counter+3)/ANTE_TILE_SIZE%ANTE_TILE_SIZE)][(counter+3)%ANTE_TILE_SIZE] = temp       & 0xff;

      counter+=4;
    }
  }

  void SingleBuffer::genMoore() {

    buffer_out[0] = buf[command_out_x][command_out_y];
    buffer_out[1] = buf[command_out_x+1][command_out_y];
    buffer_out[2] = buf[command_out_x][command_out_y+1];
    buffer_out[3] = buf[command_out_x+1][command_out_y+1];
  }
}}


