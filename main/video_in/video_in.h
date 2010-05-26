#ifndef VIDEO_IN_H
#define VIDEO_IN_H

#include <systemc>
#include"wb_slave.h"
#include"wb_master.h"

#define PIXEL_BUFFER_SIZE 1280

#define I_WIDTH     640
#define I_HEIGHT    480
#define BUFLINES    10
#define BUFSIZE    6400

//enum PixelBufferStates {
//    UNDEFINED = -1,
//    EMPTY,
//    FULL,
//    FREE,
//    USED
//};
//
//struct PixelBuffer {
//    int state;
//    char* r, w;
//    char buffer[PIXEL_BUFFER_SIZE];
//
//    void reset() {
//        state = EMPTY;
//        r = buffer;
//        w = buffer;
//    }
//};

namespace soclib { namespace caba {

    template <typename wb_param>

        class VideoIn: public sc_core::sc_module{
            protected:
                SC_HAS_PROCESS(VideoIn);
            
            
            public:
                sc_core::sc_in<bool>      p_clk;
//                sc_core::sc_in<bool>      p_video_clk;
                sc_core::sc_in<bool>      p_resetn;

                // Inputs from video_gen.
                sc_in<bool>             line_valid;
                sc_in<bool>             frame_valid;
                sc_in<unsigned char>    pixel_in;

                sc_signal<bool>         line_valid_temp;
                sc_signal<bool>         frame_valid_temp;

                WbMaster <wb_param>     p_wb_master;
                WbSlave <wb_param>      p_wb_slave;

                // constructor
                VideoIn (sc_core::sc_module_name insname);

                void sample();

                void masterTransition();
                void masterMoore();

                void slaveTransition();
                void slaveMoore();


            private:
                uint32_t    master_state, slave_state;

                char        buffer[BUFLINES * I_WIDTH];
                uint32_t    w_line, w_pixel, r_line, r_pixel;

                uint32_t    write_address;

                char        count;

                enum states {
                    UNDEFINED,
                    // Master states.
                    WAIT,
                    START_FLUSH,
                    FLUSH,
                    STOP_FLUSH,
                    // Slave states.
                    WAIT_CONFIG,
                    ACK_AND_WAIT_FRAME,
                    WAIT_FRAME,
                    RECEIVE
                };
        };

}}
#endif
