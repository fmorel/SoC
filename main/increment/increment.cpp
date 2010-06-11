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
	    min_incr.tile_ready		(signal_new_tile);
	    min_incr.ask_for_x_y	(signal_ask);
	    min_incr.p_wb_slave		(p_minincr_slave);
	    min_incr.p_wb_master		(p_minincr_master);
	    min_incr.x			(signal_x);
	    min_incr.x_min		(signal_x_min);
	    min_incr.y			(signal_y);
	    min_incr.y_min		(signal_y_min);
	    min_incr.debug_state		(signal_min_incr_debug_state);
	    min_incr.debug_signal		(signal_min_incr_debug_signal);

	    //Connection of buff_and_inter entries
	    buff_and_inter.p_clk	(p_clk);
	    buff_and_inter.p_resetn	(p_resetn);
	    buff_and_inter.y		(signal_y);
	    buff_and_inter.x		(signal_x);
	    buff_and_inter.x_min	(signal_x_min);
	    buff_and_inter.y_min	(signal_y_min);
	    buff_and_inter.tile_ready	(signal_new_tile);
	    buff_and_inter.ask_for_x_y	(signal_ask);
	    buff_and_inter.intensity	(signal_intensity);
	    buff_and_inter.valid	(signal_valid);

	  
	    buff_and_inter.p_wb_master	(p_buffer_master);
	    buff_and_inter.p_wb_slave	(p_buffer_slave);


     //outputtile module
      output_tile.p_clk(p_clk);
      output_tile.p_resetn(p_resetn);
      output_tile.p_intensity(signal_intensity);
      output_tile.p_valid(signal_valid);
      
      output_tile.p_wb_master(p_output_master);
      output_tile.p_wb_slave(p_output_slave);



	}

    template <typename wb_param> \
	void Increment<wb_param>::incrementTransition() {
	    /*DEBUG*/	x_display = signal_x;
	    /*DEBUG*/	x_min_display		    = signal_x_min;
	    /*DEBUG*/	y_display		    = signal_y;
	    /*DEBUG*/	y_min_display		    = signal_y_min;
	    /*DEBUG*/	ask_for_x_y_display	    = signal_ask;
	    /*DEBUG*/	new_tile_display	    = signal_new_tile;
	    /*DEBUG*/	intensity_display	    = signal_intensity;
	    /*DEBUG*/	valid_display		    = signal_valid;
	    /*DEBUG*/	min_incr_debug_signal	    = signal_min_incr_debug_signal;
	    /*DEBUG*/	min_incr_debug_state	    = signal_min_incr_debug_state;


	}

    template <typename wb_param> \
	void Increment<wb_param>::incrementMoore() {
	}

}}
