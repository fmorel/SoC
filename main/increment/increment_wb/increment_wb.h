#ifndef INCREMENT_WB_H
#define INCREMENT_WB_H

#include <systemc>
#include <stdint.h>
#include "wb_slave.h"
#include "wb_master.h"
#include "../../segmentation.h"

namespace soclib { namespace caba {

    template <typename wb_param>

    class WbArbiter: public sc_core::sc_module{
	private:
	    float registered_poly[20];
	protected:
	    SC_HAS_PROCESS(WbArbiter);
	public:
	    sc_core::sc_in<bool>              p_clk;
	    sc_core::sc_in<bool>              p_resetn;
	    WbSlave <wb_param>              wb_slave;
	    WbMaster <wb_param>              wb_master;

	    // interface avec le bloc incr√mentale de calcul de x
	    sc_core::sc_out<bool>              x_p_init_mux_switch;
	    sc_core::sc_out<float>             x_p_Q0;
	    sc_core::sc_out<float>             x_p_Q1;
	    sc_core::sc_out<float>             x_p_Q2;
	    sc_core::sc_out<float>             x_p_Q3;
	    sc_core::sc_out<float>             x_p_R0;
	    sc_core::sc_out<float>             x_p_R1;
	    sc_core::sc_out<float>             x_p_R2;
	    sc_core::sc_out<float>             x_p_S0;
	    sc_core::sc_out<float>             x_p_S1;
	    sc_core::sc_out<float>             x_p_P0;
	    sc_core::sc_in<float>            x_p_OUT;

	    // interface avec le bloc incr√©mental de calcul de y
	    sc_core::sc_out<bool>              y_p_init_mux_switch;
	    sc_core::sc_out<float>             y_p_Q0;
	    sc_core::sc_out<float>             y_p_Q1;
	    sc_core::sc_out<float>             y_p_Q2;
	    sc_core::sc_out<float>             y_p_Q3;
	    sc_core::sc_out<float>             y_p_R0;
	    sc_core::sc_out<float>             y_p_R1;
	    sc_core::sc_out<float>             y_p_R2;
	    sc_core::sc_out<float>             y_p_S0;
	    sc_core::sc_out<float>             y_p_S1;
	    sc_core::sc_out<float>             y_p_P0;
	    sc_core::sc_in<float>            y_p_OUT;

	    // interface avec le buffer 1
	    //
	    //Reading the buffer
	    sc_core::sc_in<unsigned char>    buffer_out[4];
	    sc_core::sc_out<int>               buffer_command_out[2];

	    //Writing to the buffer
	    sc_core::sc_out<uint32_t>           buffer_in;
	    sc_core::sc_out<bool>      buffer_write_enable;

	    // constructor
	    WbArbiter (sc_core::sc_module_name insname);

	    void WbArbiterTransition();
	    void WbArbiterMoore();
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
