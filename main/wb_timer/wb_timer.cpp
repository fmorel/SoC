#include "wb_timer.h"
#include <iostream>

namespace soclib { namespace caba {
    // Constructor
    template <typename wb_param> \
        WbTimer<wb_param>::WbTimer(sc_core::sc_module_name insname) : 
            sc_core::sc_module(insname) {
                std::cout << "WishBone Timer"  << name() 
                    << " created sucsesfully" << std::endl;

                SC_METHOD(transition);
                sensitive << p_clk.pos();

                SC_METHOD(genMealy);
                sensitive << p_clk.neg();
                sensitive << p_wb;
            }


    // Synchronoue methode
    template <typename wb_param> \
        void WbTimer<wb_param>::transition() {

            if (p_resetn == false) {
                // reset registers.Snapshot couter
                registers.Status = 0;
                registers.Control = 0;
                registers.Period = 0x0;
                registers.Snapshot = registers.Period;
#ifdef SOCLIB_MODULE_DEBUG
                std::cout << name() << " "
                    << "RESET:: waiting for requests"
                    << std::endl;
#endif
                return;
            }
            //Update the counter if enabled
            if (registers.Status & (1 << 1)) {
              registers.Snapshot = (registers.Snapshot-1);
            }
            if (p_wb.STB_I ) {
                if (p_wb.CYC_I ) {
                 //   std::cout << name() << " "
                 //       << "Recived a valid strobe" << std::endl
                  //      << p_wb
                 //       << " at registers.Snapshot "<< std::dec << registers.Snapshot
                 //       << std::endl;
                   //Writing
                    if (p_wb.WE_I) {
                      uint64_t offset = p_wb.ADR_I.read() & 0xf;
                      volatile uint64_t addr = ((uint64_t)&registers + offset);
                      uint32_t value = p_wb.DAT_I.read();
                      switch (offset) {
                        case 0x00:
#define STATUS_RW 0xfffd
                          *((uint32_t*)addr) |=  value & STATUS_RW ;
                          *((uint32_t*)addr) &=  value | ~STATUS_RW ;
                          break;
#define CONTROL_RW 0xffff
                        case 0x04: 
                          *((uint32_t*)addr) |=  value & CONTROL_RW ;
                          *((uint32_t*)addr) &=  value | ~CONTROL_RW ;
                          //write 1 to start bit
                          if (value & 1<<2) {
                            registers.Status |= 1 << 1 ;
                          } else if (value & 1<<3) { // write 1 to stop bit
                            registers.Status &= ~(1 << 1) ;
                          }
                          break;
#define PERIOD_RW 0xffff
                        case 0x08:
                          *((uint32_t*)addr) |=  value & PERIOD_RW ;
                          *((uint32_t*)addr) &=  value | ~PERIOD_RW ;
                          registers.Snapshot = registers.Period;break;
//This is readonly
#define SNAPSHOT_RW 0x0000
                        case 0x0c: 
                          *((uint32_t*)addr) |=  value & SNAPSHOT_RW ;
                          *((uint32_t*)addr) &=  value | ~SNAPSHOT_RW ;
                          break;
                        default: break;
                      }
                    }
                }
                else  {
                 //   std::cout << name() << " "
                  // /     << "Recived an invalid strobe" << std::endl
                  //      << p_wb
                  //      << " at registers.Snapshot "<< std::dec << registers.Snapshot
                  //      << std::endl;
                }
            }
            
        }

    // Synchronoue methode
    template <typename wb_param> \
        void WbTimer<wb_param>::genMealy() {
          if (registers.Control & (1 << 0) && // Interrupt enabled
              registers.Status  & (1 << 1) && // Running 
              registers.Snapshot == 0         // Counter = 0
             ) {
            p_S_INT_O = 1;
            registers.Snapshot = registers.Period;
            if (!(registers.Control & (1<<1))) {
              registers.Status &= ~(1<<1);
            }
          } else {
            p_S_INT_O = 0;
          }
            if (p_wb.STB_I ) {
                if (p_wb.CYC_I ) {
                    //Read
                    if (!p_wb.WE_I) {
                      p_wb.ACK_O = true;
                      uint64_t offset = p_wb.ADR_I.read() & 0xf;
                      volatile uint64_t addr = ((uint64_t)&registers + offset);
                      p_wb.DAT_O = *((uint32_t*)addr) ;
                    }
                }
            }
        }
}}
