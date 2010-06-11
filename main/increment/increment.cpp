#include "increment.h"
#include "increment_hard.h"
#include <iostream>

using namespace std;

namespace soclib { namespace caba {
    // Constructor
    template <typename wb_param> \
        Increment<wb_param>::Increment(sc_core::sc_module_name insname)
        : sc_core::sc_module(insname),
        incHardX("incHardX"),
        incHardY("incHardY")
//          ,interpolation("interpolation")
//          ,buffer("buffer")
    {
        std::cout << "WishBone increment"  << name() 
            << " created sucsesfully" << std::endl;

        SC_METHOD(slaveTransition);
        dont_initialize();
        sensitive << p_clk.pos();
        SC_METHOD(slaveMoore);
        dont_initialize();
        sensitive << p_clk.neg();

        SC_METHOD(masterTransition);
        dont_initialize();
        sensitive << p_clk.pos();
        SC_METHOD(masterMoore);
        dont_initialize();
        sensitive << p_clk.neg();

        incHardX.p_resetn(p_resetn);
        incHardX.p_clk(p_clk);
        incHardX.p_init_mux_switch(dummy_bool);
        incHardX.p_Q0(dummy_float);
        incHardX.p_Q1(dummy_float);
        incHardX.p_Q2(dummy_float);
        incHardX.p_Q3(dummy_float);
        incHardX.p_R0(dummy_float);
        incHardX.p_R1(dummy_float);
        incHardX.p_R2(dummy_float);
        incHardX.p_S0(dummy_float);
        incHardX.p_S1(dummy_float);
        incHardX.p_P0(dummy_float);
        incHardX.p_OUT(dummy_float1);

        incHardY.p_resetn(p_resetn);
        incHardY.p_clk(p_clk);
        incHardY.p_init_mux_switch(dummy_bool);
        incHardY.p_Q0(dummy_float);
        incHardY.p_Q1(dummy_float);
        incHardY.p_Q2(dummy_float);
        incHardY.p_Q3(dummy_float);
        incHardY.p_R0(dummy_float);
        incHardY.p_R1(dummy_float);
        incHardY.p_R2(dummy_float);
        incHardY.p_S0(dummy_float);
        incHardY.p_S1(dummy_float);
        incHardY.p_P0(dummy_float);
        incHardY.p_OUT(dummy_float1);

//        buffer.p_clk(p_clk);
//        buffer.p_resetn(p_resetn);
//        buffer.buffer_in(dummy_uint32);
//        buffer.buffer_write_enable(dummy_bool);
//        for(int i=0;i<4;i++)
//            buffer.buffer_out[i] (dummy_uchar[i]);
//        for (int i = 0;i<2;i++)
//            buffer.buffer_command_out[i] (dummy_uint32_t[i]);
//
//        interpolation.p_clk(p_clk);
//        interpolation.p_resetn(p_resetn);
//        interpolation.x(dummy_float);
//        interpolation.y(dummy_float);
//        interpolation.out(dummy_out);
//        for(int i=0;i<4;i++)
//            interpolation.buffer_in[i] (dummy_uchar[i]);
//        for (int i = 0;i<2;i++)
//            interpolation.buffer_command[i] (dummy_uint32_t[i]);

    }

    template <typename wb_param> \
        void Increment<wb_param>::slaveTransition() {
            int32_t next_state;

            // Reset handling.
            if(!p_resetn) {
                printf("INCREMENT: Reset.\n");
                slave_state = WAIT_CONFIG;
                coeff_address_start = 0;
                coeff_address_current = 0;
                return;
            }

            switch(slave_state) {

                case IDLE:
                    next_state = WAIT_CONFIG;
                    break;

                case WAIT_CONFIG:
                    if (p_wb_slave.STB_I && p_wb_slave.CYC_I) {
                        if(p_wb_slave.WE_I)
                            next_state = LOAD_COEFF;
                        else
                            printf("INCREMENT: Something tried to read us.");
                    } else {
                        next_state = WAIT_CONFIG;
                    }
                    break;

                case ACK_AND_LOAD_COEFF:
                    next_state = LOAD_COEFF;
                    break;

                case LOAD_COEFF:
                    break;

                default:
                    next_state = IDLE;
            }

            slave_state = next_state;
        }


    template <typename wb_param> \
        void Increment<wb_param>::masterTransition() {
            int32_t next_state;

            switch(master_state) {

                case IDLE:
                    break;

                case LOAD_COEFF:
                    break;

                default:
                    next_state = IDLE;
            }

            master_state = next_state;
        }


    template <typename wb_param> \
        void Increment<wb_param>::slaveMoore() {
            int32_t next_state;

            switch(slave_state) {

                case IDLE:
                    p_wb_slave.ACK_O = 0;
                    break;

                case WAIT_CONFIG:
                    p_wb_slave.ACK_O = 0;
                    break;

                case ACK_AND_LOAD_COEFF:
                    // Update the address and acknowledge the request.
                    coeff_address_start = p_wb_slave.DAT_I.read();
                    coeff_address_current = p_wb_slave.DAT_I.read();
                    cout << "INCREMENT: coeff address = " << coeff_address_start << endl;
                    // Coming from WAIT_CONFIG, we set ACK_O to 1.
                    p_wb_slave.ACK_O = 1;
                    break;

                case LOAD_COEFF:
                    break;

                default:
                    break;
            }
        }


    template <typename wb_param> \
        void Increment<wb_param>::masterMoore() {
        }

}}


