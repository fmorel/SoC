#include "calcul_incremental.h"
//#include <cstdio>
//#include <iostream>

namespace soclib { namespace caba {
    template <typename wb_param> \
	CalculInc<wb_param>::CalculInc(sc_core::sc_module_name insname) : 
	    sc_core::sc_module(insname) {

		SC_METHOD(transition);
		sensitive << p_clk.pos();

		SC_METHOD(genMealy);
		sensitive << p_clk.neg();
		sensitive << p_wb;
	    }


    // Synchronoue methode
    template <typename wb_param> \
	void CalculInc<wb_param>::transition() {
	    int registered = 0;
	    int x_step=0;
	    int y_step=0;
	    if (p_resetn == false) {
		return;
	    }
	    else if (p_wb.STB_I ) {
		if (p_wb.CYC_I ) {
		    if (p_wb.WE_I) {
			poly_reg[registered]=p_wb.DAT_I;
			registered++ ;
			registered = (registered==13 ? 0 : registered) ;
		    }
		}
	    }
	    else {
		if (y_step <16){
		    p_wb.DAT_O = poly_reg[3];
		    if(x_step==0&&y_step==0){
			//P3(X,Y+1);
			poly_reg[5]+=poly_reg[6];
			poly_reg[6]+=poly_reg[7];
			poly_reg[7]+=poly_reg[3];
			//P2(X,Y+1);
			poly_reg[9]+=poly_reg[10];
			poly_reg[10]+=poly_reg[2];
			//P1(X,Y)
			poly_reg[12]+=poly_reg[1];

		    }
		    x_step++;
		    if(x_step == 16){
			// P3(X,Y+1)
			poly_reg[3]=poly_reg[7];
			poly_reg[7]+=poly_reg[6];
			poly_reg[6]+=poly_reg[5];
			poly_reg[5]+=poly_reg[4];
			// P2(X,Y+1)
			poly_reg[2]=poly_reg[10];
			poly_reg[10]+=poly_reg[9];
			poly_reg[9]+=poly_reg[8];
			// P1(X,Y+1)
			poly_reg[1]=poly_reg[12];
			poly_reg[12]+=poly_reg[11];
			x_step=0;
			y_step++;
		    }
		    poly_reg[3]+=poly_reg[2];
		    poly_reg[2]+=poly_reg[1];
		    poly_reg[1]+=poly_reg[0];
		}
	    }
	}

    // Synchronoue methode
    template <typename wb_param> \
	void CalculInc<wb_param>::genMealy() {
	    // always acknowledge requests
	    p_wb.ACK_O = p_wb.STB_I && p_wb.STB_I;
	    // read write request counter
	}
}}
