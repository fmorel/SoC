 #include "output_tile.h"

 namespace soclib { namespace caba {


	template <typename wb_param> 
  OutputTile<wb_param>::OutputTile(sc_core::sc_module_name insname)
	    : sc_core::sc_module(insname) {

        SC_METHOD(masterTransition);
        dont_initialize();
        sensitive<<p_clk.pos();

        SC_METHOD(masterMoore);
        dont_initialize();
        sensitive<<p_clk.neg();

      SC_METHOD(slaveTransition);
      dont_initialize();
      sensitive<<p_clk.pos();
      
      SC_METHOD(slaveMoore);
      dont_initialize();
      sensitive<<p_clk.neg();


    }


  template <typename wb_param> 
    void OutputTile<wb_param>::slaveTransition() {
      if(p_resetn==false) {
        slaveState=SLAVE_IDLE;
        //random ...
        base_address=RAM_BASE+0x800;
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
  
  template <typename wb_param>
    void OutputTile<wb_param>::slaveMoore() {
      switch(slaveState) {
        case SLAVE_IDLE:
          p_wb_slave.ACK_O=0;
          break;
        //acknowledge the request and echo the address
        case SLAVE_REQ:
          p_wb_slave.ACK_O=1;
          if (p_wb_slave.WE_I)
            base_address=p_wb_slave.DAT_I.read();
          p_wb_slave.DAT_O=base_address;
      }
    }




    template <typename wb_param>
    void OutputTile<wb_param>::masterTransition() {
      if (resetn==false) {
        state=IDLE;
        nb_tile=0;
        return;
      }

      switch(state) {
        case IDLE:
          if (p_valid.read()) {
             next_state=BUFFERING;
             incomingPixel=p_intensity;
             index=0;
           }
           break;
         
         case BUFFERING:
          buffer[index]=incoming_pixel;
          index++;
          incoming_pixel=p_intensity;
          if (! p_valid.read()) {
            index=0;
            address=base_address+offset;
            outgoing_data=buffer[0]+ 
                          (buffer[1]<<8) +
                          (buffer[2]<<16)+
                          (buffer[3]<<24);
            next_state=TRANSMIT;
          }
          break;
        
        case TRANSMIT:
          if (p_wb_master.ACK_I) {
            index+=4;
            outgoing_data=buffer[index+0]+ 
                          (buffer[index+1]<<8) +
                          (buffer[index+2]<<16)+
                          (buffer[index+3]<<24);
          
            if (index==BUS_CHUNK_SIZE*4) {
              next_state=TRANSMIT_PAUSE;
              if (index==TILE_SIZE*TILE_SIZE)
                next_state=END;
          }
          break;
        case TRANSMIT_PAUSE:
          next_state=TRANSMIT;
          break;

        case END:
          offset+=TILE_SIZE;
          nb_tile++;
          if (offset%WIDTH==0)
            offset+=(TILE_SIZE-1)*WIDTH;
          if (nb_tile==WIDTH*HEIGHT/(TILE_SIZE*TILE_SIZE)) {
            nb_tile=0;
            offset=0;
          }
          next_state=IDLE;
          break;

      }   
    }

    template <typename wb_param>
    void OutputTile<wb_param>::masterMoore() {
      switch(state) {
        case IDLE:
          break;
        case BUFFERING:
          break;
        case TRANSMIT:
          p_wb_master.CYC_O=1;
          p_wb_master.STB_O=1;
          p_wb_WE_O=1;
          p_wb_ADR_O=address;
          p_wb_DAT_O=outgoing_data;
          break;
        case TRANSMIT_PAUSE:
          p_wb_master.CYC_O=0;
          p_wb_master.STB_O=0;
          break;
        case END:
          break;
    }

 }}