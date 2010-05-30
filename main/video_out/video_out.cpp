#include "video_out.h"
#include <iostream>

namespace soclib { namespace caba {
	// Constructor
	template <typename wb_param> \
		VideoOut<wb_param>::VideoOut(sc_core::sc_module_name insname)
		: sc_core::sc_module(insname)
		{
			std::cout << "WishBone video out "  << name() 
				<< " created sucsesfully" << std::endl;
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

			SC_METHOD(outputTransition);
			dont_initialize();
			sensitive<<p_video_clk.pos();
			SC_METHOD(outputMoore);
			dont_initialize();
			sensitive<<p_clk.neg();
		}

	
	template <typename wb_param> \
		void VideoOut<wb_param>::slaveTransition() {
			if(p_resetn==false) {
				slaveState=SLAVE_IDLE;
				return;
			}
			//received a valid message
			if (slaveState==SLAVE_IDLE && p_wb_slave.CYC_I && p_wb_slave.STB_I) {
				slaveState=SLAVE_REQ;
				//write address ...
			} 
			else {
				//return to idle after a request has been made
				if (slaveState ==SLAVE_REQ)
					slaveState=SLAVE_IDLE;
			}

		}
	
	template <typename wb_param> \
		void VideoOut<wb_param>::slaveMoore() {
			switch(slaveState) {
				case SLAVE_IDLE:
					p_wb_slave.ACK_O=0;
					break;
				//acknowledge the request and echo the address
				case SLAVE_REQ:
					p_wb_slave.ACK_O=1;
					if (p_wb_slave.WE_I)
						address=p_wb_slave.DAT_I.read();
					p_wb_slave.DAT_O=address;
			}
		}

	
	
	
	// transition
	template <typename wb_param> \
		void  VideoOut<wb_param>::masterTransition() {
			int data;
			//reset
			if (p_resetn == false) {
				// add initialiszations
				std::cout << name() << " Reset..."<<std::endl;
				address=0;
				writingLine=0;
				masterState=MASTER_IDLE;
				return;
			}
			switch (masterState) {

				//waits for address to be set
				case MASTER_IDLE:
					p_interrupt=0;
					// if address is not null
					if (address) {
						masterState=MASTER_BEGINLINE;
						writingLine=0;
					}
					break;
				
				//perform the initialization of a new line and checks to not overwrite buffered line
				case MASTER_BEGINLINE:
					if (writingLine < (outputLine+BUFLINE)) {
						
						masterState=MASTER_TRANS;
						writingWord=0;
					}
					break;
				
				//get from wishbone the content of one line
				case MASTER_TRANS:
					if (p_wb_master.ACK_I) {
						data=p_wb_master.DAT_I.read();
						//data is on 32 bits, we got 4 pixels here
                        if(writingLine <= 1 && writingWord <= 10) {
                            std::cout << "VIDEO_OUT:" << data << std::endl;
                        }
						lineBuffer[writingLine%BUFLINE][writingWord*4]=data>>24;
						lineBuffer[writingLine%BUFLINE][writingWord*4+1]=(data>>16) & 0xff;
						lineBuffer[writingLine%BUFLINE][writingWord*4+2]=(data>>8) & 0xff;
						lineBuffer[writingLine%BUFLINE][writingWord*4+3]=data & 0xff;
						writingWord++;
						address+=4;
						if(writingWord == WIDTH/4)
							masterState=MASTER_ENDOFLINE;
					}
					break;
				
				//relase the wishbone bus and start another line or finalize transmission
				case MASTER_ENDOFLINE:
					writingLine++;
					if (writingLine==HEIGHT)
							masterState=MASTER_ENDOFTRANS;
					else
							masterState=MASTER_BEGINLINE;
					break;
				
				//send an interrut and reset the address register when transmission has ended
				case MASTER_ENDOFTRANS:
					p_interrupt=1;
					masterState=MASTER_IDLE;
					address=0;
				}
			}

	// genMoore
	template <typename wb_param> \
		void VideoOut<wb_param>::masterMoore() {
			// on the falling edge of clk
			switch (masterState) {
				case MASTER_IDLE:
					p_wb_master.STB_O=0;
					p_wb_master.CYC_O=0;
					break;
				case MASTER_TRANS:
					p_wb_master.STB_O=1;
					p_wb_master.CYC_O=1;
					p_wb_master.WE_O=0;
					p_wb_master.ADR_O=address;
					break;
				case MASTER_ENDOFLINE:
					p_wb_master.STB_O=0;
					p_wb_master.CYC_O=0;
				default:
					p_wb_master.STB_O=0;
					p_wb_master.CYC_O=0;
			}
		}

	//outputs pixels ;)
	template <typename wb_param> \
	void VideoOut<wb_param>::outputTransition() {

		if (p_resetn ==false) 
			nextOutputState=OUTPUT_IDLE;

		switch(outputState) {

			case OUTPUT_IDLE:
				//wait for the line being correctly written
				if (outputLine < writingLine) {
					nextOutputState=OUTPUT_LINE;
					//cycle_count=3 allows to set pixel_out correctly in OUTPUT_LINE state
					cycle_count=2;
					outputPixel=0;
				}
				break;

			case OUTPUT_LINE:
				p_pixel_out.write(lineBuffer[outputLine%BUFLINE][outputPixel]);
				outputPixel++;
				//end of line is reached
				if (outputPixel == WIDTH-1) {
					cycle_count = 0;
					nextOutputState=OUTPUT_WAITLINE;
					//we need to check end of frame here so that frame_valid and line_valid resets at the same cycle
					if (outputLine== HEIGHT-1) {
						outputLine++;
						nextOutputState=OUTPUT_WAITFRAME;
					}
				}
				break;
			
			case OUTPUT_WAITLINE:
				cycle_count++;
				//wait (at least !) for LINE_SYNC cycle before new line
				if (cycle_count == LINE_SYNC) {
					outputLine++;
					//IDLE state makes the line initialization and checks if the line has been correctly written
					nextOutputState=OUTPUT_IDLE;		
				}
				break;
			
			//wait (at least) for FRAME_SYNC cycles before  new frame
			case OUTPUT_WAITFRAME:
				cycle_count++;
				if (cycle_count==FRAME_SYNC) {
					nextOutputState=OUTPUT_IDLE;
					outputLine=0;
				}
		}

		if (p_resetn==false) {
			p_line_valid=0;
			p_frame_valid=0;
		}

		switch(outputState) {
			case OUTPUT_IDLE:

			break;

			case OUTPUT_LINE:
				p_line_valid=1;
				p_frame_valid=1;
			break;

			case OUTPUT_WAITLINE:
				p_line_valid=0;
				break;

			case OUTPUT_WAITFRAME:
				p_line_valid=0;
				p_frame_valid=0;
				break;
		}
		outputState=nextOutputState;


}

	template <typename wb_param> \
	void VideoOut<wb_param>::outputMoore() {
	}
		
}}


