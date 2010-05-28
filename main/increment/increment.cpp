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

      
		}

	template <typename wb_param> \
	void Increment<wb_param>::incrementTransition() {
	}

    template <typename wb_param> \
	void Increment<wb_param>::incrementMoore() {
	}

}}


