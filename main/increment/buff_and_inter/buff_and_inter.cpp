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
	    void BufInter<wb_param>::BufInterTransition() {
		//		signal_x=x;
		//		signal_y=y;
		//		signal_x_min=x_min;
		//		signal_y_min=y_min;
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
    intensity = signal_intensity;
      }
    }
}


