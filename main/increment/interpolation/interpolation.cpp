#include "interpolation.h"
#include <iostream>

namespace soclib { namespace caba {
	// Constructor
		Interpolation::Interpolation(sc_core::sc_module_name insname)
		: sc_core::sc_module(insname)
		{
			SC_METHOD(interpolationTransition);
			dont_initialize();
			sensitive<<p_clk.pos();
			SC_METHOD(interpolationMoore);
			dont_initialize();
			sensitive<<p_clk.neg();
		}

	void Interpolation::interpolationTransition() {
    x=p_x.read();
    y=p_y.read();
    for(int i=0;i<4;i++)
      intensities[i]=p_buffer_in[i].read();
  }

	void Interpolation::interpolationMoore() {
    int Ex,Ey;
    Ex=(int)x;
    Ey=(int)y;
    float dx,dy;
    dx = x -Ex;
    dy = y- Ey;
    int temp = (1-dx)*(1-dy)*intensities[0] + (1-dx) * dy * intensities[1] + dx * ( 1 - dy ) * intensities[2] + dx * dy * intensities[3];
    p_out = temp;
    p_buffer_command[0] = Ex; 
    p_buffer_command[1] = Ey; 
  }
}}


