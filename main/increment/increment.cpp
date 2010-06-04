#include "increment.h"
#include <iostream>

namespace soclib { namespace caba {
    // Constructor
    template <typename wb_param> \
	Increment<wb_param>::Increment(sc_core::sc_module_name insname)
	: sc_core::sc_module(insname),
	min_incr("min_incr"),
	buff_and_inter("buff_and_inter"),
	new_tile_display("new_tile_display"),
	ask_for_x_y_display("ask_for_x_y_display"),
	intensity_display("intensity_display"),
	valid_display("valid_display"),
	x_display("x_display"),
	x_min_display("x_min_display"),
	y_display("y_display"),
	y_min_display("y_min_display")
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
	    min_incr.new_tile		(signal_new_tile);
	    min_incr.ask_for_x_y	(signal_ask);
	    min_incr.p_wb_slave		(p_wb_slave);
	    min_incr.x			(signal_x);
	    min_incr.x_min		(signal_x_min);
	    min_incr.y			(signal_y);
	    min_incr.y_min		(signal_y_min);

	    //Connection of buff_and_inter entries
	    buff_and_inter.p_clk	(p_clk);
	    buff_and_inter.p_resetn	(p_resetn);
	    buff_and_inter.y		(signal_y);
	    buff_and_inter.x		(signal_x);
	    buff_and_inter.x_min	(signal_x_min);
	    buff_and_inter.y_min	(signal_y_min);
	    buff_and_inter.new_tile	(signal_new_tile);
	    buff_and_inter.ask_for_x_y	(signal_ask);

	    /*DEBUG*/	buff_and_inter.x_display(x_display);
	    /*DEBUG*/	buff_and_inter.x_min_display(x_min_display);
	    /*DEBUG*/	buff_and_inter.y_display(y_display);
	    /*DEBUG*/	buff_and_inter.y_min_display(y_min_display);
	    /*DEBUG*/	buff_and_inter.intensity(intensity_display);
	    /*DEBUG*/	buff_and_inter.valid(valid_display);


	    buff_and_inter.p_wb_master	(p_wb_master);
	}

    template <typename wb_param> \
	void Increment<wb_param>::incrementTransition() {
	    new_tile_display=signal_new_tile;
	    ask_for_x_y_display=signal_ask;
	}

    template <typename wb_param> \
	void Increment<wb_param>::incrementMoore() {
	}

}}
