#ifndef VIDEO_OUT_H_
#define VIDEO_OUT_H_

#include <systemc>
#include <stdint.h>

#include "../segmentation.h"

namespace soclib { namespace caba {

    template <typename wb_param>

        class IncrementHard: public sc_core::sc_module{
            private:
						protected:
                SC_HAS_PROCESS(VideoOut);
            public:
                sc_core::sc_in<bool>              p_clk;
                sc_core::sc_in<bool>              p_resetn;
                //video interface
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
