#include "increment.h"
#include "increment_hard.h"
#include <iostream>

namespace soclib { namespace caba {
	// Constructor
	template <typename wb_param> \
		Increment<wb_param>::Increment(sc_core::sc_module_name insname)
		: sc_core::sc_module(insname)
     ,incrementHard("increment hard")
     ,interpolation("interpolation")
     ,buffer("buffer")
		{
			std::cout << "WishBone increment"  << name() 
				<< " created sucsesfully" << std::endl;

			SC_METHOD(incrementTransition);
			dont_initialize();
			sensitive<<p_clk.pos();
			SC_METHOD(incrementMoore);
			dont_initialize();
			sensitive<<p_clk.neg();

      incrementHard.p_resetn(p_resetn);
      incrementHard.p_clk(p_clk);
      incrementHard.p_init_mux_switch(dummy_bool);
      incrementHard.p_Q0(dummy_float);
      incrementHard.p_Q1(dummy_float);
      incrementHard.p_Q2(dummy_float);
      incrementHard.p_Q3(dummy_float);
      incrementHard.p_R0(dummy_float);
      incrementHard.p_R1(dummy_float);
      incrementHard.p_R2(dummy_float);
      incrementHard.p_S0(dummy_float);
      incrementHard.p_S1(dummy_float);
      incrementHard.p_P0(dummy_float);
      incrementHard.p_OUT(dummy_float1);

      buffer.p_clk(p_clk);
      buffer.p_resetn(p_resetn);
      buffer.buffer_in(dummy_uint32);
      buffer.buffer_write_enable(dummy_bool);
      for(int i=0;i<4;i++)
        buffer.buffer_out[i] (dummy_uchar[i]);
      for (int i = 0;i<2;i++)
        buffer.buffer_command_out[i] (dummy_uint32_t[i]);
      
      interpolation.p_clk(p_clk);
      interpolation.p_resetn(p_resetn);
      interpolation.x(dummy_float);
      interpolation.y(dummy_float);
      interpolation.out(dummy_out);
      for(int i=0;i<4;i++)
        interpolation.buffer_in[i] (dummy_uchar[i]);
      for (int i = 0;i<2;i++)
        interpolation.buffer_command[i] (dummy_uint32_t[i]);
      
		}

	template <typename wb_param> \
	void Increment<wb_param>::incrementTransition() {
  }

	template <typename wb_param> \
	void Increment<wb_param>::incrementMoore() {
	}
		
}}


