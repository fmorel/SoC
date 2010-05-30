#include <iostream>
#include "video_in.h"
#include "../segmentation.h"

using namespace std;

namespace soclib { namespace caba {
    // Constructor
    template <typename wb_param> \
    VideoIn<wb_param>::VideoIn(sc_core::sc_module_name insname) : 
        sc_core::sc_module(insname) {
            std::cout << "VideoIn"  << name() 
                << " created sucsesfully" << std::endl;

			SC_METHOD(sample);
			dont_initialize();
			sensitive << p_clk.pos();

//			SC_METHOD(sample_video);
//			dont_initialize();
//			sensitive << p_video_clk.pos();

			SC_METHOD(masterTransition);
			dont_initialize();
			sensitive << p_clk.pos();
			SC_METHOD(masterMoore);
			dont_initialize();
			sensitive << p_clk.neg();
			
			SC_METHOD(slaveTransition);
			dont_initialize();
			sensitive << p_clk.pos();
			SC_METHOD(slaveMoore);
			dont_initialize();
			sensitive << p_clk.neg();

        }


    template <typename wb_param> \
        void VideoIn<wb_param>::sample() {
            line_valid_temp     = line_valid;
            frame_valid_temp    = frame_valid;
            p_video_clk_temp    = p_video_clk;
            video_clk_rising    = !p_video_clk_temp && p_video_clk;
            pixel_temp = pixel_in;
            count++;
        }


//    template <typename wb_param> \
//        void VideoIn<wb_param>::sample_video() {
//        }
//

    template <typename wb_param> \
        void VideoIn<wb_param>::slaveTransition() {
            int next_state;

            // Reset handling.
            if(!p_resetn) {
                cout << "VIDEO_IN: Reset." << endl;
                slave_state = WAIT_CONFIG;
                start_address = 0;
                write_address = 0;
                count = 0;
                w_line = 0; w_pixel = 0;
                r_line = 0; r_pixel = 0;
                return;
            }

            switch(slave_state) {

                case WAIT_CONFIG:
                    if (p_wb_slave.STB_I && p_wb_slave.CYC_I) {
                        if(p_wb_slave.WE_I)     // Write
                            next_state = ACK_AND_WAIT_FRAME;
                        else    // Read (internal state through write_address)
                            next_state = ACK_READ;
                    } else {
                        next_state = WAIT_CONFIG;
                    }
                    break;

                case ACK_READ:
                    next_state = WAIT_CONFIG;
                    break;

                case ACK_AND_WAIT_FRAME:
                    next_state = WAIT_FRAME;
                    break;

                case WAIT_FRAME:
                    if(frame_valid && !frame_valid_temp) {
                        next_state = RECEIVE;
                    } else {
                        next_state = WAIT_FRAME;
                    }
                    break;

                case WAIT_LINE:
                    if(line_valid && !line_valid_temp) {
                        next_state = RECEIVE;
                    } else {
                        next_state = WAIT_LINE;
                    }
                    break;

                case RECEIVE:
                    if(!line_valid_temp) {
//                    if(!line_valid) {
//                        w_pixel = (w_pixel + 1) % I_WIDTH;
//                        cout << "VIDEO_IN: end of line. " << w_line << "  -  " << w_pixel << endl;
                        next_state = WAIT_LINE;
                    }
                    else if(!frame_valid) {
                        next_state = WAIT_CONFIG;
                    } else {
                        next_state = RECEIVE;
                    }
                    break;

                default:
                    next_state = WAIT_CONFIG;
            }


            slave_state = next_state;
        }


    template <typename wb_param> \
        void VideoIn<wb_param>::masterTransition() {
            int next_state;
            int r = I_WIDTH * r_line + r_pixel;
            int w = I_WIDTH * w_line + w_pixel;

            // Reset handling.
            if(!p_resetn) {
                master_state = WAIT;
                return;
            }

            switch(master_state) {

                case WAIT:
                    // If there is at least one line loaded in the buffer,
                    // start flushing.
                    if(w - r > I_WIDTH || (w == 0 && r > 0)) {
                        next_state = START_FLUSH;
                    } else {
                        next_state = WAIT;
                    }
                    break;
    
                case START_FLUSH:
//                    cout << "DEBUG: Start flushing." << endl;
                    next_state = FLUSH;
                    break;
    
                case FLUSH:
                    // Check if enough pixels are in the buffer.
                    // Don't stop if we are finishing to send an image to memory
                    // while waiting for the next image.
                    if(((r + 4 >= w) || (r % I_WIDTH == 0)) && !(w == 0)) {
                        next_state = STOP_FLUSH;
//                        cout << "VIDEO_IN: Stop flush. r = " << r << "  -   w = " << w << endl;
                    } else if(r >= I_WIDTH * I_HEIGHT - 1) {    // End of image.
                        cout << "VIDEO_IN: End of image." << endl;
                        next_state = STOP_FLUSH_END_IMG;
                    } else {
                        next_state = FLUSH;
                    }
                    break;
    
                case STOP_FLUSH_END_IMG:
//                    cout << "DEBUG: Stop flushing." << endl;
                    next_state = WAIT;
                    break;
    
                case STOP_FLUSH:
//                    cout << "DEBUG: Stop flushing." << endl;
                    next_state = WAIT;
                    break;
    
                default:
                    next_state = WAIT;
                    break;
 
            }


            master_state = next_state;
        }


    template <typename wb_param> \
        void VideoIn<wb_param>::slaveMoore() {

            switch(slave_state) {

                case WAIT_CONFIG:
                    p_wb_slave.ACK_O = 0;
                    w_pixel = 0;
                    w_line  = 0;
                    break;

                case ACK_AND_WAIT_FRAME:
                    // Update the address and acknowledge the reqest.
                    start_address = p_wb_slave.DAT_I.read();
                    write_address = p_wb_slave.DAT_I.read();
                    cout << "VIDEO_IN: start_address = " << start_address << endl;
                    // Make the address available for control.

                    // Coming from WAIT_CONFIG, we set ACK_O to 1.
                    p_wb_slave.ACK_O = 1;
                    count = 0;
                    break;

                case ACK_READ:
                    p_wb_slave.ACK_O = 1;
                    break;

                case WAIT_FRAME:
                    // Coming from ACK_AND_WAIT_FRAME, we set ACK_O to 0.
                    p_wb_slave.ACK_O = 0;
                    count = 0;
                    if(frame_valid && video_clk_rising) {
                        buffer[(I_WIDTH * w_line + w_pixel) % (I_WIDTH * BUFLINES)] = pixel_temp;
                        w_pixel = (w_pixel + 1) % I_WIDTH;
                        w_line = w_pixel == 0 ? (w_line + 1) % I_HEIGHT : w_line;
                    } else {
                        w_pixel = 0;
                        w_line  = 0;
                    }
                    break;

                case WAIT_LINE:
                    count = 0;
                    if(line_valid && video_clk_rising) {
                        buffer[(I_WIDTH * w_line + w_pixel) % (I_WIDTH * BUFLINES)] = pixel_temp;
                        w_pixel = (w_pixel + 1) % I_WIDTH;
                        w_line = w_pixel == 0 ? (w_line + 1) % I_HEIGHT : w_line;
                    }
                    break;

                case RECEIVE:
                    // If we are receiving a frame, store the pixel in the buffer.
                    // We sample every 4 clocks.
//                    if(count % 4 == 0) {
                    if(video_clk_rising) {
                        buffer[(I_WIDTH * w_line + w_pixel) % (I_WIDTH * BUFLINES)] = pixel_temp;
                        w_pixel = (w_pixel + 1) % I_WIDTH;
                        w_line = w_pixel == 0 ? (w_line + 1) % I_HEIGHT : w_line;
                    }
                    break;


                default:
                    break;
            }

            // Always ouput the start_address (indicates the internal state)
            // on DAT_O.
            p_wb_slave.DAT_O.write(start_address);

        }


    template <typename wb_param> \
        void VideoIn<wb_param>::masterMoore() {
            int r = I_WIDTH * r_line + r_pixel;

            switch(master_state) {

                case WAIT:
                    // If we receive a reset and come here, we need to restore
                    // these signals to 0.
                    p_wb_master.STB_O = 0;
                    p_wb_master.CYC_O = 0;
                    p_wb_master.WE_O = 0;
                    p_wb_master.SEL_O = 0;
                    interrupt = 0;
                    break;
    
                case START_FLUSH:
                    p_wb_master.STB_O = 1;
                    p_wb_master.CYC_O = 1;
                    p_wb_master.WE_O = 1;
                    p_wb_master.SEL_O = 0xF;
                    p_wb_master.ADR_O = write_address;
                    p_wb_master.DAT_O = *((int32_t*)(&buffer[r % BUFSIZE]));
                    write_address += 4;
                    // We need to be sure that I_WIDTH % 4 == 0.
                    r_pixel = (r_pixel + 4) % I_WIDTH;
                    r_line = r_pixel == 0 ? r_line + 1 : r_line;
                    break;
    
                case FLUSH:
                    if(p_wb_master.ACK_I) {
                        p_wb_master.ADR_O = write_address;
                        p_wb_master.DAT_O = *((int32_t*)(&buffer[r % BUFSIZE]));
                        write_address += 4;
                        // We need to be sure that I_WIDTH % 4 == 0.
                        r_pixel = (r_pixel + 4) % I_WIDTH;
                        r_line = r_pixel == 0 ? r_line + 1 : r_line;
                    }
                    break;
    
                case STOP_FLUSH_END_IMG:
                    interrupt = 1;
                    r_line = 0;
                    r_pixel = 0;
                    p_wb_master.STB_O = 0;
                    p_wb_master.CYC_O = 0;
                    p_wb_master.WE_O = 0;
                    p_wb_master.SEL_O = 0;
                    break;

                case STOP_FLUSH:
                    interrupt = 0;
                    p_wb_master.STB_O = 0;
                    p_wb_master.CYC_O = 0;
                    p_wb_master.WE_O = 0;
                    p_wb_master.SEL_O = 0;
                    break;

                default:
                    break;
            }
        }

}}
