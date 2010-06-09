#include "buff_and_inter.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

namespace soclib { 
    namespace caba {
	int i=0;
	bool loaded = false;
	bool interpoled = false;
	int enough = 0;

	// Constructor
	template <typename wb_param> \
	    BufInter<wb_param>::BufInter(sc_core::sc_module_name insname)
	    : sc_core::sc_module(insname),
	    buffer("buffer"),
	    interpolation("interpolation"),
	    p_clk("p_clk"),
	    p_resetn("p_resetn"),
	    x("x"),
	    x_min("x_min"),
	    y("y"),
	    y_min("y_min"),
	    new_tile("new_tile"),
	    ask_for_x_y("ask_for_x_y"),
	    intensity("intensity"),
	    valid("valid")
	{

	    SC_METHOD(BufInterTransition);
	    dont_initialize();
	    sensitive<<p_clk.pos();
	    SC_METHOD(BufInterMoore);
	    dont_initialize();
	    sensitive<<p_clk.neg();

	    buffer.p_clk(p_clk);
	    buffer.p_resetn(p_resetn);
	    interpolation.p_clk(p_clk);
	    interpolation.p_resetn(p_resetn);
	    interpolation.x(signal_x);
	    interpolation.y(signal_y);
	    interpolation.out(signal_intensity);
	    int i;
	    for(i = 0;i<2;i++){
		buffer.buffer_command_out[i](signal_buffer_command[i]);
		interpolation.buffer_command[i](signal_buffer_command[i]);
	    }
	    for(i = 0;i<4;i++){
		interpolation.buffer_in[i](signal_buffer_out[i]);
		buffer.buffer_out[i](signal_buffer_out[i]);
	    }


	    buffer.buffer_write_enable(signal_write_enable);
	    buffer.buffer_in(signal_buffer_in);
	}

	template <typename wb_param> \
	    void BufInter<wb_param>::BufInterMoore() {
		state = next_state;
		switch(state) {
		    case LOADING :
			ask_for_x_y=false;
			interpoled=false;
			//std::cout << "LOADING, i =" << i << std::endl;
			new_tile=false;
			i++;
			signal_write_enable=(i<256);
			signal_buffer_in=0x12345678+i;
			if(i==5000){
			    i=0;
			    loaded=true;
			}
			break;

		    case INTERPOLING :
			loaded=false;
			//	std::cout << "INTERPOLING, i =" << i << std::endl;
			i++;
			if(i<3){
			    ask_for_x_y = 1;
			}
			else if(i<258){
			    ask_for_x_y=false;
			    valid=true;
			}
			else {
			    valid=false;
			    ask_for_x_y=false;
			    interpoled=true;
			    new_tile=true;
			    i=0;
			}
			break;
		    case IDLE :
			break;

		    default:
			state = LOADING;
		}

	    }
	template <typename wb_param> \
	    void BufInter<wb_param>::BufInterTransition() {
		if(state==LOADING && loaded){
		    //std::cout << " Changing to interpol" << std::endl;
		    next_state=INTERPOLING;
		    enough++;
		}
		if(state==INTERPOLING && interpoled) {
		    //std::cout << " Changing to load" << std::endl;
		    next_state=LOADING;
		}
		if(enough==16)
		    next_state=IDLE;

		intensity = signal_intensity;
	    }
    }
}
