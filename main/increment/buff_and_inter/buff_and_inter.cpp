#include "buff_and_inter.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

namespace soclib { 
    namespace caba {
	int i=0;
	bool loaded = false;
	bool interpoled = false;

	// Constructor
	template <typename wb_param> \
	    BufInter<wb_param>::BufInter(sc_core::sc_module_name insname)
	    : sc_core::sc_module(insname),
	    buffer("buffer"),
	    interpolation("interpolation")
	{

	    SC_METHOD(BufInterTransition);
	    dont_initialize();
	    sensitive<<p_clk.pos();
	    SC_METHOD(BufInterMoore);
	    dont_initialize();
	    sensitive<<p_clk.neg();

	    buffer.p_clk(p_clk);
	    interpolation.p_clk(p_clk);
	    interpolation.x(x);
	    interpolation.y(y);
	    buffer.buffer_command_out[2](interpolation.buffer_command[2]);
	    interpolation.buffer_in[4](buffer.buffer_out[4]);

	    /*DEBUG*/	    x_display(x);
	    /*DEBUG*/	    x_min_display(x_min);
	    /*DEBUG*/	    y_display(y);
	    /*DEBUG*/	    y_min_display(y_min);
	    /*DEBUG*/	    new_tile_display(new_tile);
	    /*DEBUG*/	    ask_for_x_y_display(ask_for_x_y);
	    /*DEBUG*/	    intensity_display(intensity);
	    /*DEBUG*/	    valid_display(valid);

	    buffer.buffer_write_enable(signal_write_enable);
	    buffer.buffer_in(signal_buffer_in);
	}

	template <typename wb_param> \
	    void BufInter<wb_param>::BufInterTransition() {
		switch(state) {
		    case LOADING : 
			new_tile=false;
			i++;
			signal_write_enable=(i<256);
			signal_buffer_in=0x12345678;
			if(i==700){
			    i=0;
			    loaded=true;
			}
			break;

		    case INTERPOLING :
			i++;
			if(i<3){
			    ask_for_x_y = 1;
			}
			else if(i<258){
			    valid=true;
			}
			else {
			    interpoled=true;
			    new_tile=true;
			    i=0;
			}
			break;

		    default:
			state = IDLE;
		}

		next_state = state;
	    }
	template <typename wb_param> \
	    void BufInter<wb_param>::BufInterMoore() {
		if(state==LOADING && loaded)
		    next_state=INTERPOLING;
		if(state==INTERPOLING && interpoled)
		    next_state=LOADING;
	    }
    }
}


