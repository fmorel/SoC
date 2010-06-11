#include "increment_hard.h"
#include <iostream>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

// Init all registers with p_init_mux_switched to true for at least 2 cycles. Then switch it to false and read the values from p_OUT
namespace soclib { namespace caba {
    // Constructor
    IncrementHard::IncrementHard(sc_core::sc_module_name insname)
	: sc_core::sc_module(insname)
    {

	SC_METHOD(incrementHardTransition);
	dont_initialize();
	sensitive<<p_clk.pos();
	SC_METHOD(incrementHardMoore);
	dont_initialize();

	sensitive<<p_clk.neg();
    }

    void IncrementHard::incrementHardTransition() {
	if (p_init_mux_switch) {
	    cycle_since_init = 0;
	} 
	else {
	    cycle_since_init++;
	}

	if (cycle_since_init == 0) {
	    newQ0 = p_Q0.read(); 
	    newR0 = p_R0.read();
	    newS0 = p_S0.read();
	    newP0 = p_P0.read();
	    newQ1 = p_Q1.read();
	    newQ2 = p_Q2.read();
	    newQ3 = p_Q3.read();
	    newR1 = p_R1.read();
	    newR2 = p_R2.read();
	    newS1 = p_S1.read();
	    newP3 = 0;
	    newP2 = 0;
	    newP1 = 0;
	}
	else {
	    newQ0 = Q0;
	    newR0 = R0;
	    newS0 = S0;
	    newP0 = P0;
	    if (cycle_since_init%TILE_WIDTH == 1) {
		newQ1 = Q1 + Q0;
		newQ2 = Q2 + Q1;
		newQ3 = Q3 + Q2;
		newR1 = R1 + R0;
		newR2 = R2 + R1;
		newS1 = S1 + S0;
		newP3 = Q3;
		newP2 = R2;
		newP1 = S1;
	    } else {
		newQ1 = Q1;
		newQ2 = Q2;
		newQ3 = Q3;
		newR1 = R1;
		newR2 = R2;
		newS1 = S1;
		newP3 = P3 + P2;
		newP2 = P2 + P1;
		newP1 = P1 + P0;
	    }
	}

	P0 = newP0;
	P1 = newP1;
	P2 = newP2;
	P3 = newP3;
	Q0 = newQ0;
	Q1 = newQ1;
	Q2 = newQ2;
	Q3 = newQ3;
	R0 = newR0;
	R1 = newR1;
	R2 = newR2;
	S0 = newS0;
	S1 = newS1;

    }

    void IncrementHard::incrementHardMoore() {
	p_OUT = (cycle_since_init<256 ? P3 : 0) ;
    }

}}


