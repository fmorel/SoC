#include "increment_wb.h"
#include <iostream>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

namespace soclib { namespace caba {
    static int poly_index=0;
    static bool good_to_send = false;
    static int computation_cycle=0;

    // Constructor
    template <typename wb_param> \
	WbArbiter<wb_param>::WbArbiter(sc_core::sc_module_name insname)
	: sc_core::sc_module(insname)
	{

	    SC_METHOD(WbArbiterTransition);
	    dont_initialize();
	    sensitive<<p_clk.pos();
	    SC_METHOD(WbArbiterMoore);
	    dont_initialize();

	    sensitive<<p_clk.neg();
	}

    template <typename wb_param> \
    void WbArbiter<wb_param>::WbArbiterTransition() {

	if(wb_slave.STB_I && wb_slave.CYC_I && wb_slave.WE_I && (!good_to_send)){
	    registered_poly[poly_index++]= wb_slave.DAT_I;
	    if(poly_index==20) {
		poly_index = 0;
		good_to_send=true;
	    }
	}

	if((computation_cycle<2)&& good_to_send) {

	    x_p_Q0=registered_poly[0];
	    x_p_Q1=registered_poly[1];
	    x_p_Q2=registered_poly[2];
	    x_p_Q3=registered_poly[3];
	    x_p_R0=registered_poly[4];
	    x_p_R1=registered_poly[5];
	    x_p_R2=registered_poly[6];
	    x_p_S0=registered_poly[7];
	    x_p_S1=registered_poly[8];
	    x_p_P0=registered_poly[9];


	    y_p_Q0=registered_poly[10];
	    y_p_Q1=registered_poly[11];
	    y_p_Q2=registered_poly[12];
	    y_p_Q3=registered_poly[13];
	    y_p_R0=registered_poly[14];
	    y_p_R1=registered_poly[15];
	    y_p_R2=registered_poly[16];
	    y_p_S0=registered_poly[17];
	    y_p_S1=registered_poly[18];
	    y_p_P0=registered_poly[19];

	    y_p_init_mux_switch=1;
	    x_p_init_mux_switch=1;

	}
	if(computation_cycle==2){
	    y_p_init_mux_switch=0;
	    x_p_init_mux_switch=0;
	    good_to_send=false;
	}
	if(computation_cycle>=2) {

	    y_p_init_mux_switch=0;
	    x_p_init_mux_switch=0;
	}

	computation_cycle++;

	if(computation_cycle==258) {
	    computation_cycle=0;
	}
    }
    template <typename wb_param> \
    void WbArbiter<wb_param>::WbArbiterMoore() {
    }
}}


