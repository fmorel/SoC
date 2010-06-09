#ifndef BUFF_AND_INTER
#define BUFF_AND_INTER

#include <systemc>
#include <stdint.h>
#include "wb_slave.h"
#include "wb_master.h"
#include "utils.h"
#include "buffer.h"
#include "interpolation.h"
#include "segmentation.h"

namespace soclib { namespace caba {

  template <typename wb_param>
    class BufInter: public sc_core::sc_module {
      private:
       
        //buffer submodule
        soclib::caba::Buffer buffer;
        //interpolation submodule
        soclib::caba::Interpolation interpolation;
        
        //FSM states
        enum BufInterStates {
          //states of the wb_master
          WB_LOADING_START,
          WB_LOADING_WAIT,
          WB_LOADING_WRITE,
          WB_LOADING_ENDLINE,
          WB_LOADING_END,
          //states of the interpolator
          WAIT_FOR_INCR,
          INTERPOLING,
          NEW_TILE
        };

        //internal signals
        sc_signal<bool> signal_write_enable;
        sc_signal<uint32_t> signal_buffer_in;
        sc_signal<float> signal_x;
        sc_signal<float> signal_y;
        sc_signal<int>   signal_buffer_command[2];
        sc_signal<unsigned char>   signal_buffer_out[4];

        //used by the wb master
        int state;
        int next_state;
        uint32_t x_min_integer,y_min_integer;
        int loading_line,loading_word;
        uint32_t incoming_data;
        uint64_t base_address,address; 

        //used by wb_slave
        int slaveState;
        #define SLAVE_IDLE 0
        #define SLAVE_REQ 1

      protected:
        SC_HAS_PROCESS(BufInter);

      public:    
        sc_core::sc_in<bool>    p_clk;
        sc_core::sc_in<bool>    p_resetn;
        
        ////// data ports ///////
        //x and y used during interpolation phase
        sc_core::sc_in<float>   x;
        sc_core::sc_in<float>   y;
        //x_min and y_min used during the buffering  phase
        sc_core::sc_in<float>   x_min;
        sc_core::sc_in<float>   y_min;
        //the final output
        sc_core::sc_out<unsigned char>   intensity;

        ////// control ports /////
        sc_core::sc_out<bool>   new_tile;
        sc_core::sc_out<bool>   ask_for_x_y;
        sc_core::sc_out<bool>   valid;

        //wishbone master for buffering phase
        WbMaster <wb_param>		p_wb_master;

        //wishbone slave to get the address
        WbSlave <wb_param> p_wb_slave;
        
        // constructor
        BufInter (sc_core::sc_module_name insname);
        
        //master FSM methods
        void BufInterTransition();
        void BufInterMoore();

        //slave FSM methods
        void slaveTransition();
        void slaveMoore();
    };
}}
#endif

