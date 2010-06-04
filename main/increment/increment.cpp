#include "increment.h"
#include <iostream>

namespace soclib { namespace caba {
    // Constructor
    template <typename wb_param> \
	Increment<wb_param>::Increment(sc_core::sc_module_name insname)
	: sc_core::sc_module(insname),
	min_incr("min_incr"),
	buff_and_inter("buff_and_inter")
	{
	    std::cout << "WishBone increment"  << name() 
		<< " created sucsesfully" << std::endl;

	    SC_METHOD(incrementTransition);
	    dont_initialize();
	    sensitive<<p_clk.pos();
	    SC_METHOD(incrementMoore);
	    dont_initialize();
	    sensitive<<p_clk.neg();
	    
	    //Connection of min_incr entries
	    min_incr.p_clk		(p_clk);
	    min_incr.p_resetn		(p_resetn);
	    min_incr.new_tile		(buff_and_inter.new_tile);
	    min_incr.ask_for_x_y	(buff_and_inter.ask_for_x_y);
	    min_incr.p_wb_slave		(p_wb_slave);
	    
	    //Connection of buff_and_inter entries
	    buff_and_inter.p_clk	(p_clk);
	    buff_and_inter.p_resetn	(p_resetn);
	    buff_and_inter.x		(min_incr.x);
	    buff_and_inter.y		(min_incr.y);
	    buff_and_inter.x_min	(min_incr.x_min);
	    buff_and_inter.y_min	(min_incr.y_min);

	    /*DEBUG*/	    buff_and_inter.x_display(x_display);
	    /*DEBUG*/	    buff_and_inter.x_min_display(x_min_display);
	    /*DEBUG*/	    buff_and_inter.y_display(y_display);
	    /*DEBUG*/	    buff_and_inter.y_min_display(y_min_display);
	    /*DEBUG*/	    buff_and_inter.new_tile_display(new_tile_display);
	    /*DEBUG*/	    buff_and_inter.ask_for_x_y_display(ask_for_x_y_display);
	    /*DEBUG*/	    buff_and_inter.intensity_display(intensity_display);
	    /*DEBUG*/	    buff_and_inter.valid_display(valid_display);

	    buff_and_inter.p_wb_master	(p_wb_master);
	}

    template <typename wb_param> \
	void Increment<wb_param>::incrementTransition() {
	}

    template <typename wb_param> \
	void Increment<wb_param>::incrementMoore() {
	}

}}
