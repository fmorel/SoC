#ifndef BUFF_AND_INTER
#define BUFF_AND_INTER

#include <systemc>
#include <stdint.h>
#include "wb_slave.h"
#include"wb_master.h"
#include "../buffer/buffer.h"
#include "../interpolation/interpolation.h"
#include "../../segmentation.h"

namespace soclib { namespace caba {

    template <typename wb_param>

	class BufInter: public sc_core::sc_module{
	    private:
		soclib::caba::Buffer buffer;
		   soclib::caba::Interpolation interpolation;
		enum BufInterStates {
		    IDLE,
		    LOADING,
				//states of the wb_master
				WB_LOADING_START,
				WB_LOADING_WAIT,
				WB_LOADING_WRITE,
				WB_LOADING_ENDLINE,
				WB_LOADING_END,
				//states of the interpolator
		    INTERPOLING
		};
		sc_signal<bool> signal_write_enable;
		sc_signal<uint32_t> signal_buffer_in;
		sc_signal<float> signal_x_min;
		sc_signal<float> signal_y_min;
    sc_signal<int>   signal_buffer_command[2];
    sc_signal<unsigned char>   signal_buffer_out[4];
    sc_signal<unsigned char>  signal_intensity;

		//used by the wb master
		int state;
		int next_state;
		uint32_t x_min_integer,y_min_integer;
		int loading_line,loading_word;
		uint32_t incoming_data;
		uint32_t loading_base_address; 

    //used by wb_slave
    int slaveState;
    #define SLAVE_IDLE 0
    #define SLAVE_REQ 1
 
	    protected:
		SC_HAS_PROCESS(BufInter);

	    public:    
		sc_signal<float> signal_x;
		sc_signal<float> signal_y;
		sc_core::sc_in<bool>    p_clk;
		sc_core::sc_in<bool>    p_resetn;
		sc_core::sc_in<float>   x;
		sc_core::sc_in<float>   x_min;
		sc_core::sc_in<float>   y;
		sc_core::sc_in<float>   y_min;
		sc_core::sc_out<bool>   new_tile;
		sc_core::sc_out<bool>   ask_for_x_y;
		sc_core::sc_out<char>   intensity;
		sc_core::sc_out<bool>   valid;

		//wishbone interface
		WbMaster <wb_param>		p_wb_master;

    //wisshbone slave to get the address
    WbSlave <wb_param> p_wb_slave;
		// constructor
		BufInter (sc_core::sc_module_name insname);
		void BufInterTransition();
		void BufInterMoore();
	};
}}
#endif

