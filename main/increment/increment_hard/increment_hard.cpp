#include "increment_hard.h"
#include <iostream>

namespace soclib { namespace caba {
	// Constructor
	template <typename wb_param> \
		VideoOut<wb_param>::IncrementHard(sc_core::sc_module_name insname)
		: sc_core::sc_module(insname)
		{

			SC_METHOD(outputTransition);
			dont_initialize();
			sensitive<<p_video_clk.pos();
			SC_METHOD(outputMoore);
			dont_initialize();
			sensitive<<p_clk.neg();
		}

	template <typename wb_param> \
	void IncrementHard<wb_param>::incrementHardTransition() {
  }

	template <typename wb_param> \
	void IncrementHard<wb_param>::incrementHardMoore() {
	}
		
}}


