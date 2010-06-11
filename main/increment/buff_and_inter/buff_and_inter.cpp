#include "buff_and_inter.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

namespace soclib { 
  namespace caba {
    int i=0;
    bool loaded = false;
    bool interpoled = false;
    int enough = 0;

    // Constructor
    template <typename wb_param> \
      BufInter<wb_param>::BufInter(sc_core::sc_module_name insname)
      : sc_core::sc_module(insname),
      buffer("buffer"),
      interpolation("interpolation"),
      p_clk("p_clk"),
      p_resetn("p_resetn"),
      x("x"),
      y("y"),
      x_min("x_min"),
      y_min("y_min"),
      intensity("intensity"),
      tile_ready("tile_ready"),
      ask_for_x_y("ask_for_x_y"),
      valid("valid")
    {

      SC_METHOD(BufInterTransition);
      dont_initialize();
      sensitive<<p_clk.pos();
      SC_METHOD(BufInterMoore);
      dont_initialize();
      sensitive<<p_clk.neg();


      SC_METHOD(slaveTransition);
      dont_initialize();
      sensitive<<p_clk.pos();

      SC_METHOD(slaveMoore);
      dont_initialize();
      sensitive<<p_clk.neg();


      buffer.p_clk(p_clk);
      buffer.p_resetn(p_resetn);
      interpolation.p_clk(p_clk);
      interpolation.p_resetn(p_resetn);
      interpolation.p_x(signal_x);
      interpolation.p_y(signal_y);
      interpolation.p_out(intensity);
      int i;
      for(i = 0;i<2;i++){
        buffer.buffer_command_out[i](signal_buffer_command[i]);
        interpolation.p_buffer_command[i](signal_buffer_command[i]);
      }
      for(i = 0;i<4;i++){
        interpolation.p_buffer_in[i](signal_buffer_out[i]);
        buffer.buffer_out[i](signal_buffer_out[i]);
      }

      buffer.buffer_write_enable(signal_write_enable);
      buffer.buffer_in(signal_buffer_in);
    }


    template <typename wb_param> 
      void BufInter<wb_param>::slaveTransition() {
        if(p_resetn==false) {
          slaveState=SLAVE_IDLE;
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
      void BufInter<wb_param>::slaveMoore() {
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

    template <typename wb_param> \
      void BufInter<wb_param>::BufInterTransition() {
        if (p_resetn == false ) {
          next_state=NEW_TILE;
          state=NEW_TILE;
          return;
        }

        switch(state) {

          case WB_LOADING_START:
            x_min_integer=x_min.read();
            //pad the x_min value to the beginning of a word
            //i.e remove the last two bits
            x_min_integer &= 0x1111111C;
            y_min_integer=y_min.read();

            loading_word=0;
            loading_line=0;
            address=base_address+x_min_integer+WIDTH*y_min_integer;
            isWriting=0;
            next_state=WB_LOADING_WAIT;
            break;

          case WB_LOADING_WAIT:
            if (p_wb_master.ACK_I) {
              incoming_data=p_wb_master.DAT_I.read();
              loading_word++;
              address+=4;
              isWriting=1;
              //end of a 32-pixel line
              if (loading_word==8) {
                loading_word=0;
                address-=32;
                loading_line++;
                address+=WIDTH;
                if (loading_line==32)
                  next_state=WB_LOADING_END;
                else
                  next_state=WB_LOADING_ENDLINE;
              }
              else
                next_state=WB_LOADING_WAIT;
            }
            else
                isWriting=0;
            break;

          case WB_LOADING_ENDLINE:
            next_state=WB_LOADING_WAIT;
            break;

          case WB_LOADING_END:
            i=0;
            next_state=WAIT_FOR_INCR;
            break;

          case WAIT_FOR_INCR:
            i++;
            //TODO: needs to be ajusted
            if (i==3) {
              i=0;
              next_state=INTERPOLING;
            }
            break;

          case INTERPOLING:
            i++;
            //TODO: needs to be adjusted
            if (i==257)
              next_state=NEW_TILE;
            break;  

          case NEW_TILE:
            if (tile_ready.read())
              next_state=WB_LOADING_START;

        }

        state=next_state;
      }

    template <typename wb_param> \
      void BufInter<wb_param>::BufInterMoore() {
        //		signal_x=x;
        //		signal_y=y;
        //		signal_x_min=x_min;
        //		signal_y_min=y_min;
        switch(state) {

          /*************************************************
           ************* wishbone master states *************
           **************************************************/

          case WB_LOADING_START:
            valid=0;
            break;

          case WB_LOADING_WAIT:
            p_wb_master.CYC_O=1;
            p_wb_master.STB_O=1;
            p_wb_master.WE_O=0;
            p_wb_master.ADR_O=address;
            signal_write_enable=isWriting;
            signal_buffer_in=incoming_data;
            break;


          case WB_LOADING_ENDLINE:
            p_wb_master.CYC_O=0;
            p_wb_master.STB_O=0;
            signal_write_enable=0;
            break;

          case WB_LOADING_END:
            p_wb_master.CYC_O=0;
            p_wb_master.STB_O=0;
            signal_write_enable=0;
            ask_for_x_y=1;
            break;


            /************ Interpolation states **********/

          case WAIT_FOR_INCR:
            ask_for_x_y = 0;
            break;

          case INTERPOLING:
            valid=1;
            break;

          case NEW_TILE:
            valid=0;


        }
      }

  }
}

