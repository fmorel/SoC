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
    int Ex,Ey;
    Ex = (int)x;
    Ey = (int)y;
    buffer_command[0] = Ex; 
    buffer_command[1] = Ey; 
  }

	void Interpolation::interpolationMoore() {
    int temp = buffer_in[0] + buffer_in[1] + buffer_in[2] + buffer_in[3];
    out = temp/4;
  }
}}


