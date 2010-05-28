#ifndef WB_SIMPLE_MASTER_H_
#define WB_SIMPLE_MASTER_H_

#include <systemc>
#include"wb_master.h"

namespace soclib { namespace caba {

    template <typename wb_param>

        class WbSimpleMaster: public sc_core::sc_module{
            protected:
                SC_HAS_PROCESS(WbSimpleMaster);
            public:
                sc_core::sc_in<bool>               p_clk;
                sc_core::sc_in<bool>               p_resetn;
                WbMaster <wb_param>    p_wb;
                // constructor
                WbSimpleMaster (sc_core::sc_module_name insname);

                void transition();
                void genMoore();
            private:
                uint32_t cycle;
                uint32_t etat;
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
