#ifndef INCREMENT_H
#define INCREMENT_H

#include <systemc>
#include "wb_master.h"
#include "wb_slave.h"
#include <stdint.h>

#include "../segmentation.h"
#include "increment_hard.h"
#include "interpolation.h"
#include "buffer.h"

namespace soclib { namespace caba {

    template <typename wb_param>

        class Increment: public sc_core::sc_module{
            private:
                soclib::caba::IncrementHard incrementHard;
                soclib::caba::Interpolation interpolation;
                soclib::caba::Buffer buffer;
                sc_core::sc_in<bool>              dummy;
                sc_core::sc_in<float>             dummy1;
						protected:
                SC_HAS_PROCESS(Increment);
            public:
                sc_core::sc_in<bool>              p_clk;
                sc_core::sc_in<bool>              p_resetn;
								//wishbone interface
								WbMaster <wb_param>    						p_wb_master;
								WbSlave <wb_param>								p_wb_slave;
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
