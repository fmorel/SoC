#include "calcul_incremental.h"
//#include <cstdio>
//#include <iostream>

static int state = 0;
static int data_out;

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
	    static int registered = 0;
	    static int x_step=0;
	    static int y_step=0;
	    static int recep_ok=0;
	    if (p_resetn == false) {
		state = 0;
		return;
	    }
	    if ((p_wb.STB_I)&&recep_ok==0) {
		if (p_wb.CYC_I ) {
		    if (p_wb.WE_I) {
			state = 1;
			poly_reg[registered]=p_wb.DAT_I;
			data_out = 0x11000000+registered+(recep_ok<<4);
			registered++ ;
			if(registered > 0xC){
			    recep_ok=1;
			    registered = 0;
			}
			
		    }
		}
	    }
	    if(recep_ok){
		state = 2;
		if (y_step <16){
		    data_out = poly_reg[3];
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
			if(y_step==16){
			    x_step = 0;
			    y_step = 0;
			    recep_ok=0;
			}
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
	    switch (state) {
		case 0 : p_wb.DAT_O =0xbbb; break;
		case 1 : p_wb.DAT_O = data_out; break;
		case 2 : p_wb.DAT_O = data_out; break ;
		default : p_wb.DAT_O = 0x12345678; break;
	    }
	    // read write request counter
	}
}}
