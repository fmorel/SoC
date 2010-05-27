#include "increment.h"
#include <iostream>

namespace soclib { namespace caba {
	// Constructor
	template <typename wb_param> \
		Increment<wb_param>::Increment(sc_core::sc_module_name insname)
		: sc_core::sc_module(insname)
		{
			std::cout << "WishBone increment"  << name() 
				<< " created sucsesfully" << std::endl;

			SC_METHOD(outputTransition);
			dont_initialize();
			sensitive<<p_clk.pos();
			SC_METHOD(outputMoore);
			dont_initialize();
			sensitive<<p_clk.neg();
		}

	template <typename wb_param> \
	void Increment<wb_param>::incrementTransition() {
  }

	template <typename wb_param> \
	void Increment<wb_param>::incrementMoore() {
	}
		
}}


