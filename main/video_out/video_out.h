#ifndef VIDEO_OUT_H_
#define VIDEO_OUT_H_

#include <systemc>
#include "wb_master.h"
#include "wb_slave.h"
#include <stdint.h>

#include "segmentation.h"
#include "utils.h"

//state of the master FSM
#define MASTER_IDLE 0
#define MASTER_BEGINLINE 1
#define MASTER_TRANS 2
#define MASTER_ENDOFLINE 3
#define MASTER_ENDOFTRANS 4

//state of the slave FSM
#define SLAVE_IDLE 0
#define SLAVE_REQ 1

//state of the output
#define OUTPUT_IDLE 0
#define OUTPUT_LINE 1
#define OUTPUT_WAITLINE 2
#define OUTPUT_WAITFRAME1 3
#define OUTPUT_WAITFRAME2 4

//synchronisation duration (in cycles)
#define LINE_SYNC 160		
#define FRAME_SYNC (WIDTH +LINE_SYNC)*40


//number of lines buffered
#define BUFLINE VIDEO_OUT_BUFSIZE

namespace soclib { namespace caba {

    template <typename wb_param>

        class VideoOut: public sc_core::sc_module{
            private:
							int masterState,slaveState;
							int outputState,nextOutputState;
							int cycle_count;
							uint64_t address;
							unsigned char lineBuffer[BUFLINE][WIDTH];
							
							int writingLine, writingWord;
							int outputLine, outputPixel;
						protected:
                SC_HAS_PROCESS(VideoOut);
            public:
                sc_core::sc_in<bool>              p_clk;
                sc_core::sc_in<bool>              p_video_clk;
                sc_core::sc_in<bool>              p_resetn;
                //video interface
								sc_core::sc_out<bool>							p_line_valid;
								sc_core::sc_out<bool>							p_frame_valid;
								sc_core::sc_out<unsigned char>		p_pixel_out;
								//wishbone interface
								WbMaster <wb_param>    						p_wb_master;
								WbSlave <wb_param>								p_wb_slave;
								//interruption when finished
								sc_core::sc_out<bool>							p_interrupt;
                // constructor
                VideoOut (sc_core::sc_module_name insname);

                void masterTransition();
                void masterMoore();

								void slaveTransition();
								void slaveMoore();

								void outputTransition();
								void outputMoore();
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
