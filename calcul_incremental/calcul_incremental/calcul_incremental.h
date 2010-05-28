#ifndef CALCUL_INCREMENTAL_H
#define CALCUL_INCREMENTAL_H

//#define SC_INCLUDE_FX
#include <systemc.h>
#include "wb_slave.h"


namespace soclib { namespace caba {

    template <typename wb_param>

	class CalculInc: public sc_core::sc_module{

	    protected:
		SC_HAS_PROCESS(CalculInc);
	    public:
		WbSlave <wb_param>    p_wb;
		sc_in_clk           p_clk;
		sc_in<bool>         p_resetn;

		////////////////////////////////////////////////////
		//	constructor
		////////////////////////////////////////////////////

		CalculInc(sc_module_name insnname);

		////////////////////////////////////////////////////
		//	methods and structural parameters
		////////////////////////////////////////////////////

		void transition();
		void genMealy();

	    private:
		const char *base_name;
		// paramètres de l'image
		sc_dt::sc_uint<32> poly_reg[13];

	};

}}
#endif
