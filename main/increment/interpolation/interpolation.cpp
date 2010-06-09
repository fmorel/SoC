#include "interpolation.h"
#include <iostream>

using namespace std;
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
    prevprevx = prevx;
    prevprevy = prevy;
    prevx=x;
    prevy=y;
    x=p_x.read();
    y=p_y.read();
    for(int i=0;i<4;i++)
      intensities[i]=p_buffer_in[i].read();
  }

	void Interpolation::interpolationMoore() {
    int Ex,Ey;
    Ex=(int)prevprevx;
    Ey=(int)prevprevy;
    float dx,dy;
    dx = prevprevx-Ex;
    dy = prevprevy-Ey;
    cout << "dx dy" << dx << "," << dy << endl;
    cout << (int)intensities[0] <<(int) intensities[1] << (int)intensities[2] <<(int) intensities[3] <<endl;
    float temp = (1-dx)*(1-dy)*intensities[0] + (1-dx) * dy * intensities[1] + dx * ( 1 - dy ) * intensities[2] + dx * dy * intensities[3];
    cout << "and temp is : " << temp << endl;
    int temp1 = (int)temp;
    cout << "and temp1 is : " << temp1 << endl;
    p_out = temp1;

    Ex=(int)x;
    Ey=(int)y;
    p_buffer_command[0] = Ex; 
    p_buffer_command[1] = Ey; 

  }
}}


