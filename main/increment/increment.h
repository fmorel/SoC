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

#define N_COEFFS 10
#define COEFF_TYPE float

namespace soclib { namespace caba {

    template <typename wb_param>
    class Increment: public sc_core::sc_module{
        private:
            soclib::caba::IncrementHard incHardX;
            soclib::caba::IncrementHard incHardY;
//            sc_signal<COEFF_TYPE> XQ0, XQ1, XQ2, XQ3, XR0, XR1, XR2, XS0, XS1, XP3, XP2, XP1, XP0;
//            sc_signal<COEFF_TYPE> YQ0, YQ1, YQ2, YQ3, YR0, YR1, YR2, YS0, YS1, YP3, YP2, YP1, YP0;
            sc_signal<COEFF_TYPE> XYcoeffs[2 * N_COEFFS];
            sc_signal<bool> Xswitch, Yswitch;

//            soclib::caba::Interpolation interpolation;
//            soclib::caba::Buffer buffer;

            sc_signal<COEFF_TYPE> dummy_float,dummy_float1;
            sc_signal<bool> dummy_bool;
            sc_signal<unsigned char> dummy_uchar[4];
            sc_signal<uint32_t> dummy_uint32;
            sc_signal<uint32_t> dummy_uint32_t[2];
            sc_signal<unsigned char> dummy_out;

            int32_t     slave_state, master_state;
            uint32_t    coeff_address_start, coeff_address_current;
            int32_t     coeff_count;

        protected:
            SC_HAS_PROCESS(Increment);
        public:
            sc_core::sc_in<bool>    p_clk;
            sc_core::sc_in<bool>    p_resetn;
            // Wishbone interface
            WbMaster <wb_param>     p_wb_master;
            WbSlave <wb_param>      p_wb_slave;
            // Constructor
            Increment (sc_core::sc_module_name insname);

            void slaveTransition();
            void slaveMoore();

            void masterTransition();
            void masterMoore();

            enum states {
                WAIT_CONFIG,
                ACK_AND_BUSY,
                BUSY,

                IDLE,
                START_LOADING_COEFF,
                LOAD_COEFF,
                DONE_LOADING_COEFF,

                COMPUTE_MIN,
                INCR
            };
    };

}}

#endif
