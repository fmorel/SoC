#include "min_incr.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

namespace soclib { 
    namespace caba {
	// Constructor
	template <typename wb_param> \
	    MinIncr<wb_param>::MinIncr(sc_core::sc_module_name insname)
	    : sc_core::sc_module(insname)
	      ,x_incrementHard("x_incrementHard")
	      ,y_incrementHard("y_incrementHard")
	      ,x_min_incrementHard("x_min_incrementHard")
	      ,y_min_incrementHard("y_min_incrementHard")
	      ,poly_index(0)
	      ,good_to_send(false)
	      ,last_init_poly(0)
	      ,next_tile_happened(true)
	      ,unvalid(true)
	{

	    SC_METHOD(MinIncrTransition);
	    dont_initialize();
	    sensitive<<p_clk.pos();
	    SC_METHOD(MinIncrMoore);
	    dont_initialize();
	    sensitive<<p_clk.neg();
	    SC_METHOD(MinIncrComputeMin);
	    dont_initialize();
	    sensitive<<p_clk.neg();

	    x_incrementHard.p_clk(p_clk);
	    x_incrementHard.p_resetn(p_resetn);
	    y_incrementHard.p_clk(p_clk);
	    y_incrementHard.p_resetn(p_resetn);
	    x_min_incrementHard.p_clk(p_clk);
	    x_min_incrementHard.p_resetn(p_resetn);
	    y_min_incrementHard.p_clk(p_clk);
	    y_min_incrementHard.p_resetn(p_resetn);

	    x_incrementHard.p_init_mux_switch	    (signal_x_mux);
	    x_min_incrementHard.p_init_mux_switch   (signal_x_min_mux);
	    y_min_incrementHard.p_init_mux_switch   (signal_y_min_mux);
	    y_incrementHard.p_init_mux_switch	    (signal_y_mux);

	    x_min_incrementHard.p_Q0(signal_x_y_min[0]);
	    x_min_incrementHard.p_Q1(signal_x_y_min[1]);
	    x_min_incrementHard.p_Q2(signal_x_y_min[2]);
	    x_min_incrementHard.p_Q3(signal_x_y_min[3]);
	    x_min_incrementHard.p_R0(signal_x_y_min[4]);
	    x_min_incrementHard.p_R1(signal_x_y_min[5]);
	    x_min_incrementHard.p_R2(signal_x_y_min[6]);
	    x_min_incrementHard.p_S0(signal_x_y_min[7]);
	    x_min_incrementHard.p_S1(signal_x_y_min[8]);
	    x_min_incrementHard.p_P0(signal_x_y_min[9]);

	    y_min_incrementHard.p_Q0(signal_x_y_min[10]);
	    y_min_incrementHard.p_Q1(signal_x_y_min[11]);
	    y_min_incrementHard.p_Q2(signal_x_y_min[12]);
	    y_min_incrementHard.p_Q3(signal_x_y_min[13]);
	    y_min_incrementHard.p_R0(signal_x_y_min[14]);
	    y_min_incrementHard.p_R1(signal_x_y_min[15]);
	    y_min_incrementHard.p_R2(signal_x_y_min[16]);
	    y_min_incrementHard.p_S0(signal_x_y_min[17]);
	    y_min_incrementHard.p_S1(signal_x_y_min[18]);
	    y_min_incrementHard.p_P0(signal_x_y_min[19]);

	    x_incrementHard.p_Q0(signal_x_y[0]);
	    x_incrementHard.p_Q1(signal_x_y[1]);
	    x_incrementHard.p_Q2(signal_x_y[2]);
	    x_incrementHard.p_Q3(signal_x_y[3]);
	    x_incrementHard.p_R0(signal_x_y[4]);
	    x_incrementHard.p_R1(signal_x_y[5]);
	    x_incrementHard.p_R2(signal_x_y[6]);
	    x_incrementHard.p_S0(signal_x_y[7]);
	    x_incrementHard.p_S1(signal_x_y[8]);
	    x_incrementHard.p_P0(signal_x_y[9]);

	    y_incrementHard.p_Q0(signal_x_y[10]);
	    y_incrementHard.p_Q1(signal_x_y[11]);
	    y_incrementHard.p_Q2(signal_x_y[12]);
	    y_incrementHard.p_Q3(signal_x_y[13]);
	    y_incrementHard.p_R0(signal_x_y[14]);
	    y_incrementHard.p_R1(signal_x_y[15]);
	    y_incrementHard.p_R2(signal_x_y[16]);
	    y_incrementHard.p_S0(signal_x_y[17]);
	    y_incrementHard.p_S1(signal_x_y[18]);
	    y_incrementHard.p_P0(signal_x_y[19]);

	    x_incrementHard.p_OUT(signal_x_tmp);
	    y_incrementHard.p_OUT(signal_y_tmp);
	    x_min_incrementHard.p_OUT(signal_x_min_tmp);
	    y_min_incrementHard.p_OUT(signal_y_min_tmp);
	}

	template <typename wb_param> \
	    void MinIncr<wb_param>::MinIncrComputeMin() {
		if((signal_x_min_tmp<x_min_internal) || unvalid)
		    x_min_internal=signal_x_min_tmp;
		if((signal_y_min_tmp<y_min_internal) || unvalid){
		    y_min_internal=signal_y_min_tmp;
		    unvalid=false;
		}
		if(new_tile)
		{
//		    x_min=x_min_internal;
//		    y_min=y_min_internal;
		    unvalid=true;
		}
		x = signal_x_tmp;
		y = signal_y_tmp;
		x_min = signal_x_min_tmp;
		y_min = signal_y_min_tmp;
		if(((int)signal_x_tmp+(int)signal_y_tmp+(int)signal_x_min_tmp+(int)signal_y_min_tmp) > 0)
		    std::cout << (int)signal_x_tmp << " " << (int)signal_y_tmp << " " 
			<< (int)signal_x_min_tmp << " " << (int)signal_y_min_tmp << std::endl;
	    }

	template <typename wb_param> \
	    void MinIncr<wb_param>::MinIncrMoore() {
		switch(state) {
		    case WAIT_CONFIG:
			debug_state = 1;
			if(p_wb_slave.STB_I && p_wb_slave.CYC_I && p_wb_slave.WE_I && (!good_to_send)){
			    //TODO : check cast is ok
			    registered_poly[last_init_poly][poly_index++]= p_wb_slave.DAT_I.read();
			    if(poly_index==20) {
				poly_index = 0;
				good_to_send=true;
			    }
			}
			signal_x_min_mux=false;
			signal_y_min_mux=false;
			signal_x_mux=false;
			signal_y_mux=false;
			init_latency=3;
			break;

		    case INIT_INCR_MIN:
			debug_state = 2;
			next_tile_happened=false;
			good_to_send=false;
			init_latency--;
			for(i = 0;i<20;i++){
			    signal_x_y_min[i]=registered_poly[last_init_poly][i];
			}
			signal_y_min_mux=true;
			signal_x_min_mux=true;
			break;

		    case INIT_INCR:
			debug_state= 3;
			init_latency--;
			for(i = 1;i<20;i++){
			    signal_x_y[i]=registered_poly[!last_init_poly][i];
			}
			signal_y_mux=true;
			signal_x_mux=true;

			break;
		    case NEW_TILE_REQUEST:
			debug_state = 4;
			last_init_poly=!last_init_poly;
			next_tile_happened=true;
			break;

		}
		
		debug_signal = poly_index + (good_to_send <<4) + (next_tile_happened << 8) + (unvalid<<12) 
			+ (last_init_poly<<16) + (init_latency<<20);
		state = next_state;
		p_wb_slave.ACK_O = p_wb_slave.STB_I && p_wb_slave.STB_I;

	    }
	template <typename wb_param> \
	    void MinIncr<wb_param>::MinIncrTransition() {

		//TODO: gros bug: on modfie next_state mais l'affectation est fait au front montant suivant..?
		// Est-qu'on peut dépendre de new_tile et ask_for_x_y dans cette partie ??
		//	std::cout << "and state== "<< state<<" and good_to_send == "<< good_to_send
		//	    << " and next_tile_happened == " << next_tile_happened << std::endl;
		if(state==INIT_INCR_MIN && init_latency==1) {
		    next_state=WAIT_CONFIG;
		} 
		if(state==INIT_INCR && init_latency==1) {
		    next_state=WAIT_CONFIG;
		}
		if(state==WAIT_CONFIG && good_to_send==true && next_tile_happened==true) {
		    next_state=INIT_INCR_MIN;
		}
		if(state==NEW_TILE_REQUEST)
		    next_state=WAIT_CONFIG;
		if(new_tile) {
		    next_state=NEW_TILE_REQUEST;
		}
		if(state==WAIT_CONFIG && ask_for_x_y) {
		    next_state=INIT_INCR;
		}
	    }
    }
}


