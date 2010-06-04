#ifndef INCREMENT_H
#define INCREMENT_H

#include <systemc>
#include "wb_master.h"
#include "wb_slave.h"
#include <stdint.h>

#include "../segmentation.h"
#include "./min_incr/min_incr.h"
#include "./buff_and_inter/buff_and_inter.h"

namespace soclib { namespace caba {

    template <typename wb_param>

        class Increment: public sc_core::sc_module{
	    private:
		soclib::caba::MinIncr <wb_param>    min_incr;
		soclib::caba::BufInter <wb_param>   buff_and_inter;
		sc_core::sc_in<bool>		    dummy;
		sc_core::sc_in<float>		    dummy1;
	    protected:
		SC_HAS_PROCESS(Increment);
	    public:
		sc_core::sc_in<bool>              p_clk;
		sc_core::sc_in<bool>              p_resetn;

		/*DEBUG*/	    sc_core::sc_inout<float>	x_display;
		/*DEBUG*/	    sc_core::sc_inout<float>	x_min_display;
		/*DEBUG*/	    sc_core::sc_inout<float>	y_display;
		/*DEBUG*/	    sc_core::sc_inout<float>	y_min_display;
		/*DEBUG*/	    sc_core::sc_inout<bool>	new_tile_display;
		/*DEBUG*/	    sc_core::sc_inout<bool>	ask_for_x_y_display;
		/*DEBUG*/	    sc_core::sc_inout<char>	intensity_display;
		/*DEBUG*/	    sc_core::sc_inout<bool>	valid_display;

		//wishbone interface
		WbMaster <wb_param>		  p_wb_master;
		WbSlave <wb_param>		  p_wb_slave;
		// constructor
		Increment (sc_core::sc_module_name insname);

		void incrementTransition();
		void incrementMoore();
	};

}}
#endif

// Local Variables:
// tab-width: 4
// c-basic-offset: 4
// c-file-offsets:((innamespace . 0)(inline-open . 0))
// indent-tabs-mode: nil
// End:

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4
