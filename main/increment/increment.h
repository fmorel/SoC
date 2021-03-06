#ifndef INCREMENT_H
#define INCREMENT_H

#include <systemc>
#include "wb_master.h"
#include "wb_slave.h"
#include <stdint.h>

#include "segmentation.h"
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
                sc_signal<float> dummy_float,dummy_float1;
                sc_signal<bool> dummy_bool;
                sc_signal<unsigned char> dummy_uchar[4];
                sc_signal<uint32_t> dummy_uint32;
                sc_signal<uint32_t> dummy_uint32_t[2];
                sc_signal<unsigned char> dummy_out;
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
