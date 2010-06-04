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
		sc_signal<bool> signal_new_tile;
		sc_signal<bool> signal_ask;
		sc_signal<char> signal_intensity;
		sc_signal<bool> signal_valid;
		sc_signal<float> signal_x;
		sc_signal<float> signal_x_min;
		sc_signal<float> signal_y;
		sc_signal<float> signal_y_min;

	    protected:
		SC_HAS_PROCESS(Increment);
	    public:
		sc_core::sc_in<bool>              p_clk;
		sc_core::sc_in<bool>              p_resetn;

		/*DEBUG*/	    sc_core::sc_out<bool>	new_tile_display;
		/*DEBUG*/	    sc_core::sc_out<bool>	ask_for_x_y_display;
		/*DEBUG*/	    sc_core::sc_out<char>	intensity_display;
		/*DEBUG*/	    sc_core::sc_out<bool>	valid_display;
		/*DEBUG*/	    sc_core::sc_out<float>	x_display;
		/*DEBUG*/	    sc_core::sc_out<float>	x_min_display;
		/*DEBUG*/	    sc_core::sc_out<float>	y_display;
		/*DEBUG*/	    sc_core::sc_out<float>	y_min_display;

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
