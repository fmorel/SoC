#ifndef INCREMENT_WB_H
#define INCREMENT_WB_H

#include <systemc>
#include <stdint.h>

#include "../../segmentation.h"

namespace soclib { namespace caba {

    class WbArbiter: public sc_core::sc_module{
	private:

	protected:
	    SC_HAS_PROCESS(WbArbiter);
	public:
	    sc_core::sc_in<bool>              p_clk;
	    sc_core::sc_in<bool>              p_resetn;
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
