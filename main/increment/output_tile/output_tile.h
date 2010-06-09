#ifndef OUTPUT_TILE
#define OUTPUT_TILE


#include <systemc>
#include <stdint.h>
#include "utils.h"
#include "wb_slave.h"
#include "wb_master.h"



#define IDLE 0
#define BUFFERING 1
#define TRANSMIT 2
#define TRANSMIT_PAUSE 3
#define END 4

#define SLAVE_IDLE 0
#define SLAVE_REQ 1

namespace soclib { namespace caba {

  template <typename wb_param>
  class OutputTile:public sc_core::sc_module {

    private:
      int state,next_state;
      uint32_t buffer[TILE_SIZE*TILE_SIZE];
      uint32_t base_address,address,offset;
      int nb_tile;
      uint32_t outgoing_data;
      unsigned char incoming_pixel;
    protected:
      SC_HAS_PROCESS(OutputTile);
    public:
      //usual ports
      sc_core::sc_in<bool> p_clk;
      sc_core::sc_in<bool> p_resetn;

      //pixels coming from interpolation
      sc_core::sc_in<unsigned char> p_intensity;
      sc_core::sc_in<unsigned char> p_valid;
  
      //master to output the tile
      WbMaster <wb_param> p_wb_master;
      //slave to get the base address of the image
      WbSlave  <wb_param> p_wb_slave;

      OutputTile(sc_core::sc_module_name insname);

      void masterTransition();
      void masterMoore();

      void slaveTransition();
      void slaveMoore();
  };

}}
      

#endif
