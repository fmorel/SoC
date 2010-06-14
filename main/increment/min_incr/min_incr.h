#ifndef MIN_INCR_H
#define MIN_INCR_H

#include <systemc>
#include <stdint.h>
#include "wb_slave.h"
#include "wb_master.h"
#include "../../segmentation.h"
#include "../increment_hard/increment_hard.h"

namespace soclib { namespace caba {

    template <typename wb_param>

	class MinIncr: public sc_core::sc_module{
	    private:
		soclib::caba::IncrementHard x_incrementHard;
		soclib::caba::IncrementHard y_incrementHard;
		soclib::caba::IncrementHard x_min_incrementHard;
		soclib::caba::IncrementHard y_min_incrementHard;
		float registered_poly[1][20];

		sc_signal<bool> signal_x_min_mux;
		sc_signal<bool> signal_y_min_mux;
		sc_signal<bool> signal_x_mux;
		sc_signal<bool> signal_y_mux;
		sc_signal<float> signal_x_y_min[20]; 
		sc_signal<float> signal_x_y[20]; 

		float x_min_internal,y_min_internal;

		int next_state;
		int i;
		bool unvalid;

		enum MinIncrStates {
		    WAIT_CONFIG,
        WB_LOADING_START,
        WB_LOADING_WAIT,
        WB_LOADING_WRITE,
        WB_LOADING_ENDLINE,
        WB_LOADING_END,
		    INIT_INCR_MIN,
        WAIT_COMPUTE_MIN,
        WAIT_X_Y,
		    INIT_INCR,
        WAIT_COMPUTE,
		};


		int state;
    int nb_tiles;
    int slaveState;

    uint32_t base_address,address;
	    protected:
		SC_HAS_PROCESS(MinIncr);

	    public:    
		sc_core::sc_signal<float> signal_x_tmp;
		sc_core::sc_signal<float> signal_y_tmp;
		sc_core::sc_signal<float> signal_x_min_tmp;
		sc_core::sc_signal<float> signal_y_min_tmp;
		sc_core::sc_signal<int> min_incr_status;
		
		sc_core::sc_in<bool>    p_clk;
		sc_core::sc_in<bool>    p_resetn;
		sc_core::sc_out<int>	debug_state;
		sc_core::sc_out<int>	debug_signal;
		sc_core::sc_out<float>  x_min;
		sc_core::sc_out<float>  y_min;
		sc_core::sc_out<float>  x;
		sc_core::sc_out<float>  y;
		sc_core::sc_out<bool>    tile_ready;
		sc_core::sc_in<bool>    ask_for_x_y;

		//wishbone interface
		WbSlave <wb_param>		  p_wb_slave;
    WbMaster <wb_param>      p_wb_master;
		// constructor
		MinIncr (sc_core::sc_module_name insname);
		void MinIncrComputeMin();
		void MinIncrTransition();
		void MinIncrMoore();
    void slaveMoore();
    void slaveTransition();
	};
}}
#endif

