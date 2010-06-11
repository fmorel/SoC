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

#define N_COEFF 20
#define COEFF_TYPE int32_t

namespace soclib { namespace caba {

    template <typename wb_param>
    class Increment: public sc_core::sc_module{
        private:
            soclib::caba::IncrementHard incHardX;
            soclib::caba::IncrementHard incHardY;
//            soclib::caba::Interpolation interpolation;
//            soclib::caba::Buffer buffer;
            sc_signal<float> dummy_float,dummy_float1;
            sc_signal<bool> dummy_bool;
            sc_signal<unsigned char> dummy_uchar[4];
            sc_signal<uint32_t> dummy_uint32;
            sc_signal<uint32_t> dummy_uint32_t[2];
            sc_signal<unsigned char> dummy_out;

            int32_t     slave_state, master_state;
            uint32_t    coeff_address_start, coeff_address_current;

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
                IDLE,
                WAIT_CONFIG,
                ACK_AND_LOAD_COEFF,
                LOAD_COEFF,
                COMPUTE_MIN,
                INCR
            };
    };

}}

#endif
