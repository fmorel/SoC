#include "wb_simple_master.h"
#include <iostream>


namespace soclib { namespace caba {
  // Constructor
  template <typename wb_param> \
    WbSimpleMaster<wb_param>::WbSimpleMaster(sc_core::sc_module_name insname)
    : sc_core::sc_module(insname)
    {
      std::cout << "WishBone simple Master "  << name() 
        << " created sucsesfully" << std::endl;
      SC_METHOD(transition);
      dont_initialize();
      sensitive << p_clk.pos();
      SC_METHOD(genMoore);
      dont_initialize();
      sensitive << p_clk.neg();
    }

  // transition
  template <typename wb_param> \
    void  WbSimpleMaster<wb_param>::transition() {
      //reset
      if (p_resetn == false) {
        // add initialiszations
        cycle = 0;
        etat = 0 ;
        //     std::cout << name() << " Reset..."<<std::endl;
        return;
      }
      cycle++;
      switch(etat) {
        case 0:
        //  if(cycle % 0x10000 == 0) {
          if (false){
            etat = 1;
          }
          break;
        case 1:
          std::cout << std::endl << "Master will write" << std::endl;
          etat = 2;
          break;
        case 3:
          if (p_wb.ACK_I==1) {
            std::cout << "Master wrote: " << std::endl;
            etat = 4;
            std::cout << "MAster will negate STB_0" << std::endl;
          }
          break;
        case 5: 
          std::cout << std::endl << "Master will read" << std::endl;
          etat = 6;
          break;
        case 7: 
          if (p_wb.ACK_I == 1) {
            std::cout << "Master read" << p_wb.DAT_I << std::endl;
            etat = 8;
            std::cout << "MAster will negate STB_0" << std::endl;
          }
          break;
      }
      // rising edge of clk
      //   std::cout << name() 
      //  << " Rising edge of clk"<<std::endl;
    }

  // genMoore
  template <typename wb_param> \
    void WbSimpleMaster<wb_param>::genMoore() {
      // on the falling edge of clk
      switch(etat) {
        case 0: break;
        case 2:
                //Write 0.
                p_wb.STB_O = 1;
                p_wb.CYC_O = 1;
                p_wb.WE_O = 1;
                p_wb.DAT_O = 0;
                p_wb.ADR_O = 0xA0000200 ;
                p_wb.SEL_O = 0;
                etat = 3;
                break;
        case 4:
                //Negate STB_O after ACK
                p_wb.STB_O = 0;
                etat = 5;
                break;
        case 6:
                p_wb.STB_O = 1;
                p_wb.CYC_O = 1;
                p_wb.WE_O = 0;
                p_wb.ADR_O = 0xA0000200 ;
                p_wb.SEL_O = 0;
                etat = 7;
                break;
        case 8:
                //Negate STB_O after ACK
                p_wb.STB_O = 0;
                etat = 0;
                break;
      }
    }

}}

// Local Variables:
// tab-width: 4
// c-basic-offset: 4
// c-file-offsets:((innamespace . 0)(inline-open . 0))
// indent-tabs-mode: nil
// End:

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4
