#include "min_incr.h"
#include "utils.h"

#define SLAVE_IDLE 0
#define SLAVE_REQ 1

namespace soclib { 
  namespace caba {
    // Constructor
    template <typename wb_param> \
      MinIncr<wb_param>::MinIncr(sc_core::sc_module_name insname)
      : sc_core::sc_module(insname)
        ,x_incrementHard("x_incrementHard")
        ,y_incrementHard("y_incrementHard")
        ,x_min_incrementHard("x_min_incrementHard")
        ,y_min_incrementHard("y_min_incrementHard")
        ,unvalid(true)
    {

      SC_METHOD(slaveTransition);
      dont_initialize();
      sensitive<<p_clk.pos();
      SC_METHOD(slaveMoore);
      dont_initialize();
      sensitive<<p_clk.neg();

      SC_METHOD(MinIncrTransition);
      dont_initialize();
      sensitive<<p_clk.pos();
      SC_METHOD(MinIncrMoore);
      dont_initialize();
      sensitive<<p_clk.neg();
      SC_METHOD(MinIncrComputeMin);
      dont_initialize();
      sensitive<<p_clk.neg();

      x_incrementHard.p_clk(p_clk);
      x_incrementHard.p_resetn(p_resetn);
      y_incrementHard.p_clk(p_clk);
      y_incrementHard.p_resetn(p_resetn);
      x_min_incrementHard.p_clk(p_clk);
      x_min_incrementHard.p_resetn(p_resetn);
      y_min_incrementHard.p_clk(p_clk);
      y_min_incrementHard.p_resetn(p_resetn);

      x_incrementHard.p_init_mux_switch	    (signal_x_mux);
      x_min_incrementHard.p_init_mux_switch   (signal_x_min_mux);
      y_min_incrementHard.p_init_mux_switch   (signal_y_min_mux);
      y_incrementHard.p_init_mux_switch	    (signal_y_mux);

      x_min_incrementHard.p_Q0(signal_x_y_min[0]);
      x_min_incrementHard.p_Q1(signal_x_y_min[1]);
      x_min_incrementHard.p_Q2(signal_x_y_min[2]);
      x_min_incrementHard.p_Q3(signal_x_y_min[3]);
      x_min_incrementHard.p_R0(signal_x_y_min[4]);
      x_min_incrementHard.p_R1(signal_x_y_min[5]);
      x_min_incrementHard.p_R2(signal_x_y_min[6]);
      x_min_incrementHard.p_S0(signal_x_y_min[7]);
      x_min_incrementHard.p_S1(signal_x_y_min[8]);
      x_min_incrementHard.p_P0(signal_x_y_min[9]);

      y_min_incrementHard.p_Q0(signal_x_y_min[10]);
      y_min_incrementHard.p_Q1(signal_x_y_min[11]);
      y_min_incrementHard.p_Q2(signal_x_y_min[12]);
      y_min_incrementHard.p_Q3(signal_x_y_min[13]);
      y_min_incrementHard.p_R0(signal_x_y_min[14]);
      y_min_incrementHard.p_R1(signal_x_y_min[15]);
      y_min_incrementHard.p_R2(signal_x_y_min[16]);
      y_min_incrementHard.p_S0(signal_x_y_min[17]);
      y_min_incrementHard.p_S1(signal_x_y_min[18]);
      y_min_incrementHard.p_P0(signal_x_y_min[19]);

      x_incrementHard.p_Q0(signal_x_y[0]);
      x_incrementHard.p_Q1(signal_x_y[1]);
      x_incrementHard.p_Q2(signal_x_y[2]);
      x_incrementHard.p_Q3(signal_x_y[3]);
      x_incrementHard.p_R0(signal_x_y[4]);
      x_incrementHard.p_R1(signal_x_y[5]);
      x_incrementHard.p_R2(signal_x_y[6]);
      x_incrementHard.p_S0(signal_x_y[7]);
      x_incrementHard.p_S1(signal_x_y[8]);
      x_incrementHard.p_P0(signal_x_y[9]);

      y_incrementHard.p_Q0(signal_x_y[10]);
      y_incrementHard.p_Q1(signal_x_y[11]);
      y_incrementHard.p_Q2(signal_x_y[12]);
      y_incrementHard.p_Q3(signal_x_y[13]);
      y_incrementHard.p_R0(signal_x_y[14]);
      y_incrementHard.p_R1(signal_x_y[15]);
      y_incrementHard.p_R2(signal_x_y[16]);
      y_incrementHard.p_S0(signal_x_y[17]);
      y_incrementHard.p_S1(signal_x_y[18]);
      y_incrementHard.p_P0(signal_x_y[19]);
      x_incrementHard.p_OUT(signal_x_tmp);
      y_incrementHard.p_OUT(signal_y_tmp);
      x_min_incrementHard.p_OUT(signal_x_min_tmp);
      y_min_incrementHard.p_OUT(signal_y_min_tmp);
    }

    template <typename wb_param> \
      void MinIncr<wb_param>::MinIncrComputeMin() {
        if((signal_x_min_tmp<x_min_internal) || unvalid)
          x_min_internal=signal_x_min_tmp;
        if((signal_y_min_tmp<y_min_internal) || unvalid){
          y_min_internal=signal_y_min_tmp;
          unvalid=false;
        }
        /*		if(new_tile)
        //		{
        //		    x_min=x_min_internal;
        //		    y_min=y_min_internal;
        unvalid=true;
        }
         */
        x = signal_x_tmp;
        y = signal_y_tmp;
        x_min = signal_x_min_tmp;
        y_min = signal_y_min_tmp;

      }

    template <typename wb_param> \
    void MinIncr<wb_param>::slaveTransition() {
        if(p_resetn==false) {
          slaveState=SLAVE_IDLE;
          base_address=0;
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
    void MinIncr<wb_param>::slaveMoore() {
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
      void MinIncr<wb_param>::MinIncrMoore() {
        switch(state) {

          /*************************************************
           ************* wishbone master states *************
           **************************************************/

          case WB_LOADING_START:
            tile_ready=0;
            break;

          case WB_LOADING_WAIT:
            p_wb_master.CYC_O=1;
            p_wb_master.STB_O=1;
            p_wb_master.WE_O=0;
            p_wb_master.ADR_O=address;
            break;

          case WB_LOADING_WRITE:
            break;

          case WB_LOADING_ENDLINE:
            p_wb_master.CYC_O=0;
            p_wb_master.STB_O=0;
            break;

          case WB_LOADING_END:
            p_wb_master.CYC_O=0;
            p_wb_master.STB_O=0;
            break;


          case INIT_INCR_MIN:
            for(int j = 0;j<20;j++){
              signal_x_y_min[j]=registered_poly[0][j];
            }
            signal_y_min_mux=true;
            signal_x_min_mux=true;
            break;

          case WAIT_COMPUTE_MIN:
            signal_x_min_mux=false;
            signal_y_min_mux=false;
            break;

          case WAIT_X_Y:
            tile_ready=true;
            break;

          case INIT_INCR:
            tile_ready=false;
            for(int j = 0;j<20;j++){
              signal_x_y[j]=registered_poly[0][j];
            }
            signal_y_mux=true;
            signal_x_mux=true;
            break;

          case WAIT_COMPUTE:
            signal_y_mux=false;
            signal_x_mux=false;
            break;

        }
      }

        template <typename wb_param> \
          void MinIncr<wb_param>::MinIncrTransition() {
              if (p_resetn ==false) {
                state=WAIT_CONFIG;
                return;
              }

            switch(state) {
              case WAIT_CONFIG:
                if (base_address) {
                  next_state=WB_LOADING_START;
                }
                break;

              case WB_LOADING_START:
                i=0;
                //for a complete image
                //address=base_address+nb_tiles*20*4;
                //for bullshit
                address=base_address;
                next_state=WB_LOADING_WAIT;
                break;

              case WB_LOADING_WAIT:
                if (p_wb_master.ACK_I) {
                  registered_poly[0][i]=p_wb_master.DAT_I.read();
                  next_state=WB_LOADING_WRITE;
                }
                break;

              case WB_LOADING_WRITE:
                address+=4;
                i++;
                //end of a 32-pixel line
                if (i%8==0) 
                  next_state=WB_LOADING_ENDLINE;
                else if (i==20)
                  next_state=WB_LOADING_END;
                else
                  next_state=WB_LOADING_WAIT;
                break;

              case WB_LOADING_ENDLINE:
                next_state=WB_LOADING_WAIT;
                break;

              case WB_LOADING_END:
                i=0;
                next_state=INIT_INCR_MIN;
                break;

              case INIT_INCR_MIN:
                i++;
                if (i==2) {
                  i=0;
                  next_state=WAIT_COMPUTE_MIN;
                }
                break;

              case WAIT_COMPUTE_MIN:
                i++;
                //TODO : tuning
                if (i==257 ) {
                  next_state=WAIT_X_Y;
                  i=0;
                }
                break;

              case WAIT_X_Y:
                if (ask_for_x_y)
                  next_state=INIT_INCR;
                break;

              case INIT_INCR:
                i++;
                if (i==2) {
                  i=0;
                  next_state=WAIT_COMPUTE;
                }
                break;

              case WAIT_COMPUTE:
                i++;
                //TODO : tuning
                if (i==257) {
                  nb_tiles++;
                  if (nb_tiles == NB_TILES)
                    address=base_address;
                  next_state=WAIT_CONFIG;
                  i=0;
                }
                break;
            }
            state=next_state;
            debug_state=state;
          }
      }
  }


