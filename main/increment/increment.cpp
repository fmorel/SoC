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

        SC_METHOD(computeMins);
        dont_initialize();
        sensitive << p_clk.pos();

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
        incHardX.p_init_mux_switch(Xswitch);
        incHardX.p_Q0(XYcoeffs[0]);
        incHardX.p_Q1(XYcoeffs[1]);
        incHardX.p_Q2(XYcoeffs[2]);
        incHardX.p_Q3(XYcoeffs[3]);
        incHardX.p_R0(XYcoeffs[4]);
        incHardX.p_R1(XYcoeffs[5]);
        incHardX.p_R2(XYcoeffs[6]);
        incHardX.p_S0(XYcoeffs[7]);
        incHardX.p_S1(XYcoeffs[8]);
        incHardX.p_P0(XYcoeffs[9]);
        incHardX.p_OUT(outX);

        incHardY.p_resetn(p_resetn);
        incHardY.p_clk(p_clk);
        incHardY.p_init_mux_switch(Yswitch);
        incHardY.p_Q0(XYcoeffs[10]);
        incHardY.p_Q1(XYcoeffs[11]);
        incHardY.p_Q2(XYcoeffs[12]);
        incHardY.p_Q3(XYcoeffs[13]);
        incHardY.p_R0(XYcoeffs[14]);
        incHardY.p_R1(XYcoeffs[15]);
        incHardY.p_R2(XYcoeffs[16]);
        incHardY.p_S0(XYcoeffs[17]);
        incHardY.p_S1(XYcoeffs[18]);
        incHardY.p_P0(XYcoeffs[19]);
        incHardY.p_OUT(outY);


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
        void Increment<wb_param>::computeMins() {

            // Reset handling.
            if(!p_resetn) {
                min_count = 0;
                minX = WIDTH * HEIGHT;
                minY = WIDTH * HEIGHT;
                return;
            }

            if(computeMinsEnabled) {
                min_count++;
                minX = minX < outX ? minX : outX;
                minY = minY < outY ? minY : outY;
                cout << "outX = " << outX << " - outY = " << outY << endl;
            } else {
                min_count = 0;
                minX = WIDTH * HEIGHT;
                minY = WIDTH * HEIGHT;
            }
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

                case WAIT_CONFIG:
                    if (p_wb_slave.STB_I && p_wb_slave.CYC_I) {
                        if(p_wb_slave.WE_I)
                            next_state = ACK_AND_BUSY;
                        else
                            printf("INCREMENT: Something tried to read us.");
                    } else {
                        next_state = WAIT_CONFIG;
                    }
                    break;

                case ACK_AND_BUSY:
                    next_state = BUSY;
                    break;

                case BUSY:
                    if(coeff_address_start == 0 && coeff_address_current == 0) {
                        next_state = WAIT_CONFIG;
                    } else {
                        next_state = BUSY;
                    }
                    break;

                default:
                    next_state = WAIT_CONFIG;
            }

            slave_state = next_state;
        }


    template <typename wb_param> \
        void Increment<wb_param>::masterTransition() {
            int32_t next_state;

            // Reset handling.
            if(!p_resetn) {
                computeMinsEnabled = false;
                master_state = IDLE;
                coeff_count = 0;
                return;
            }

            switch(master_state) {

                case IDLE:
                    coeff_count = 0;
                    if( coeff_address_start != 0) {
                        next_state = START_LOADING_COEFF;
                    } else {
                        next_state = IDLE;
                    }
                    break;

                case START_LOADING_COEFF:
                    cout << "INCREMENT: Start loading coeffs" << endl;
                    next_state = LOAD_COEFF;
                    break;

                case LOAD_COEFF:
                    if(coeff_count >= 2 * N_COEFFS) {
                        next_state = DONE_LOADING_COEFF;
                    } else {
                        next_state = LOAD_COEFF;
                        if(p_wb_master.ACK_I) {
                            XYcoeffs[coeff_count] = static_cast<COEFF_TYPE>(p_wb_master.DAT_I.read());
                            coeff_count++;
                            coeff_address_current += sizeof(COEFF_TYPE);
                            cout << "coeff_count = " << coeff_count << " -  value = " << p_wb_master.DAT_I.read() << endl;
                        }
                    }
                    break;

                case DONE_LOADING_COEFF:
                    cout << "INCREMENT: Done loading coeffs" << endl;
                    next_state = IDLE;
                    break;

                default:
                    next_state = IDLE;
            }

            master_state = next_state;
        }


    template <typename wb_param> \
        void Increment<wb_param>::slaveMoore() {

            switch(slave_state) {

                case WAIT_CONFIG:
                    p_wb_slave.ACK_O = 0;
                    break;

                case ACK_AND_BUSY:
                    cout << "INCREMENT: Got an address." << endl;
                    // Update the address and acknowledge the request.
                    coeff_address_start = p_wb_slave.DAT_I.read();
                    coeff_address_current = p_wb_slave.DAT_I.read();
                    cout << "INCREMENT: coeff address = " << coeff_address_start << endl;
                    // Coming from WAIT_CONFIG, we set ACK_O to 1.
                    p_wb_slave.ACK_O = 1;
                    break;

                case BUSY:
                    p_wb_slave.ACK_O = 0;
                    break;

                default:
                    break;
            }
        }


    template <typename wb_param> \
        void Increment<wb_param>::masterMoore() {

            if(min_count == TILE_SIZE * TILE_SIZE) {
                computeMinsEnabled = false;
                cout << "minX = " << minX << " - minY = " << minY << endl;
            }

            switch(master_state) {

                case IDLE:
                    coeff_address_start = 0;
                    coeff_address_current = 0;
                    Xswitch = 1;
                    Yswitch = 1;
                    coeff_count = 0;
                    p_wb_master.STB_O = 0;
                    p_wb_master.CYC_O = 0;
                    p_wb_master.WE_O = 0;
                    p_wb_master.SEL_O = 0x0;
                    break;

                case START_LOADING_COEFF:
                    Xswitch = 1;
                    Yswitch = 1;
                    p_wb_master.ADR_O = coeff_address_current;
                    p_wb_master.STB_O = 1;
                    p_wb_master.CYC_O = 1;
                    p_wb_master.WE_O = 0;
                    p_wb_master.SEL_O = 0xF;
                    break;

                case LOAD_COEFF:
                    p_wb_master.ADR_O = coeff_address_current;
                    break;

                case DONE_LOADING_COEFF:
                    coeff_address_start = 0;
                    coeff_address_current = 0;
                    Xswitch = 0;
                    Yswitch = 0;
                    computeMinsEnabled = true;
                    coeff_count = 0;
                    p_wb_master.STB_O = 0;
                    p_wb_master.CYC_O = 0;
                    p_wb_master.WE_O = 0;
                    p_wb_master.SEL_O = 0x0;
                    break;

                default:
                    break;
            }
        }

}}


