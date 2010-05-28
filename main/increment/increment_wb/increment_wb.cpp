#include "increment_wb.h"
#include <iostream>

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

// Init all registers with p_init_mux_switched to true for at least 2 cycles. Then switch it to false and read the values from p_OUT
namespace soclib { namespace caba {
  // Constructor
  WbArbiter::WbArbiter(sc_core::sc_module_name insname)
    : sc_core::sc_module(insname)
  {

    SC_METHOD(WbArbiterTransition);
    dont_initialize();
    sensitive<<p_clk.pos();
    SC_METHOD(WbArbiterMoore);
    dont_initialize();

    sensitive<<p_clk.neg();
  }

  void WbArbiter::WbArbiterTransition() {
  }

  void WbArbiter::WbArbiterMoore() {
  }

}}


