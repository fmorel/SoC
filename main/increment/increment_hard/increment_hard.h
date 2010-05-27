#ifndef INCREMENT_HARD_H
#define INCREMENT_HARD_H

#include <systemc>
#include <stdint.h>

#include "../../segmentation.h"

namespace soclib { namespace caba {

        class IncrementHard: public sc_core::sc_module{
            private:
              float Q0,Q1,Q2,Q3,R0,R1,R2,S0,S1,P3,P2,P1,P0;
              float newQ0,newQ1,newQ2,newQ3,newR0,newR1,newR2,newS0,newS1,newP3,newP2,newP1,newP0;
              bool p_line_mux_switch;
              int cycle_since_init;
						protected:
                SC_HAS_PROCESS(IncrementHard);
            public:
                sc_core::sc_in<bool>              p_clk;
                sc_core::sc_in<bool>              p_resetn;
                sc_core::sc_in<bool>              p_init_mux_switch;
                sc_core::sc_in<float>             p_Q0;
                sc_core::sc_in<float>             p_Q1;
                sc_core::sc_in<float>             p_Q2;
                sc_core::sc_in<float>             p_Q3;
                sc_core::sc_in<float>             p_R0;
                sc_core::sc_in<float>             p_R1;
                sc_core::sc_in<float>             p_R2;
                sc_core::sc_in<float>             p_S0;
                sc_core::sc_in<float>             p_S1;
                sc_core::sc_in<float>             p_P0;
                sc_core::sc_out<float>            p_OUT;
                // constructor
                IncrementHard (sc_core::sc_module_name insname);

								void incrementHardTransition();
								void incrementHardMoore();
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
