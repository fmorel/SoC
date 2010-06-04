#ifndef VIDEO_IN_H
#define VIDEO_IN_H

#include <systemc>
#include"wb_slave.h"
#include"wb_master.h"

#include "utils.h"

#define BUFLINES        2
#define BUFSIZE         1280 
#define MINFLUSHSIZE    4


namespace soclib { namespace caba {

    template <typename wb_param>

        class VideoIn: public sc_core::sc_module{
            protected:
                SC_HAS_PROCESS(VideoIn);
            
            
            public:
                sc_core::sc_in<bool>    p_clk;
                sc_core::sc_in<bool>    p_video_clk;
                sc_core::sc_in<bool>    p_resetn;

                // Inputs from video_gen.
                sc_in<bool>             line_valid;
                sc_in<bool>             frame_valid;
                sc_in<unsigned char>    pixel_in;

                // Interrupt signal.
                sc_out<bool>            interrupt;

                sc_signal<bool>         p_video_clk_temp;
                sc_signal<bool>         line_valid_temp;
                sc_signal<bool>         frame_valid_temp;

                // Wishbone master and slave.
                WbMaster <wb_param>     p_wb_master;
                WbSlave <wb_param>      p_wb_slave;

                // Constructor.
                VideoIn (sc_core::sc_module_name insname);

                // Functions.
                void sample();

                void masterTransition();
                void masterMoore();

                void slaveTransition();
                void slaveMoore();


            private:
                uint32_t    master_state, slave_state;

                char        buffer[BUFLINES * WIDTH];
                int32_t     w_line, w_pixel, r_line, r_pixel;

                uint32_t    start_address, write_address;

                char        count;

                char        pixel_temp;

                bool        video_clk_rising;

                enum states {
                    UNDEFINED,
                    // Master states.
                    WAIT,
                    START_FLUSH,
                    FLUSH,
                    STOP_FLUSH,
                    STOP_FLUSH_END_IMG,
                    // Slave states.
                    WAIT_CONFIG,
                    ACK_READ,
                    ACK_AND_WAIT_FRAME,
                    WAIT_FRAME,
                    WAIT_LINE,
                    RECEIVE
                };
        };

}}
#endif
