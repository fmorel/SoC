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
		    LOADING,
		    INTERPOLING,
		    IDLE
		};
		sc_signal<bool> signal_write_enable;
		sc_signal<uint32_t> signal_buffer_in;
		sc_signal<float> signal_x_min;
		sc_signal<float> signal_y_min;
		sc_signal<int>   signal_buffer_command[2];
		sc_signal<unsigned char>   signal_buffer_out[4];
		sc_signal<unsigned char>  signal_intensity;
		int state;
		int next_state;
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
		// constructor
		BufInter (sc_core::sc_module_name insname);
		void BufInterTransition();
		void BufInterMoore();
	};
}}
#endif

