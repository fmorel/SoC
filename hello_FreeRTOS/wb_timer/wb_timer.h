#ifndef WB_TIMER_H_
#define WB_TIMER_H_

#include <systemc>
#include "wb_slave.h"
#include "Timer.h"

namespace soclib { namespace caba {

    template <typename wb_param>

        class WbTimer: public sc_core::sc_module{
            protected:
                SC_HAS_PROCESS(WbTimer);
            public:
                sc_core::sc_in<bool>               p_clk;
                sc_core::sc_in<bool>               p_resetn;
                sc_core::sc_out<bool>              p_S_INT_O;
                WbSlave <wb_param>    p_wb;
                // constructor
                WbTimer (sc_core::sc_module_name insname);

                void transition();
                void genMealy();
                //void genMoore();
            private:
                uint32_t cycle;
                Timer_t registers;
        };

}}
#endif
