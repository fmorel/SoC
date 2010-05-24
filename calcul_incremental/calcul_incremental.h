#ifndef CALCUL_INCREMENTAL_H
#define CALCUL_INCREMENTAL_H

#define SC_INCLUDE_FX
#define N_poly 23

#include "systemc.h"
#include"wb_slave.h"

namespace soclib { namespace caba {

    template <typename wb_param>

	class CalculInc: public sc_core::sc_module{

	    protected:
		SC_HAS_PROCESS(CalculInc);
	    public:
		WbSlave <wb_param>    p_wb;
		sc_in_clk           p_clk;
		sc_in<bool>         p_resetn;

		sc_in<  sc_fixed_fast<10,32> > poly;
		sc_in<  bool > new_poly;
		sc_out< sc_fixed_fast<10,32> > x;
		sc_out< sc_fixed_fast<10,32> > y;

		////////////////////////////////////////////////////
		//	constructor
		////////////////////////////////////////////////////

		CalculInc(sc_module_name insnname);

		////////////////////////////////////////////////////
		//	methods and structural parameters
		////////////////////////////////////////////////////

		void transition();
		void genMealy();
		//void genMoore();

	    private:
		const char *base_name;
		// paramètres de l'image
		sc_fixed_fast<10,32> poly_reg[13];
		const uint32_t  t_WIDTH      ;
		const uint32_t  t_HEIGHT     ;
		const uint32_t  nb_t_WIDTH      ;
		const uint32_t  nb_t_HEIGHT     ;

	};

}}
#endif
