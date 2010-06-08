
// C/C++ std libs
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <systemc>

#include "utils.h"
#include "interpolation.h"

using namespace soclib::caba;
using namespace sc_core;

int _main(int argc, char* argv[]) {

  Interpolation interpolation("interpolation");

  sc_time     clk_periode(10, SC_NS); // clk period
  sc_clock	signal_clk("signal_clk",clk_periode);
  sc_signal<bool> signal_resetn("signal_resetn");

  sc_signal<float>             signal_x;
  sc_signal<float>             signal_y;
  sc_signal<unsigned char>     signal_out;

  sc_signal<unsigned char>     signal_buffer_in[4];
  sc_signal<uint32_t>          signal_buffer_command[2];

  interpolation.p_clk(signal_clk);
  interpolation.p_resetn(signal_resetn);

  interpolation.x(signal_x);
  interpolation.y(signal_y);

  interpolation.out(signal_out);

  for (int i = 0;i<4;i++) {
    interpolation.buffer_in[i](signal_buffer_in[i]);
  }
  for (int i = 0;i<2;i++) {
    interpolation.buffer_command[i](signal_buffer_command[i]);
  }

  sc_start(sc_core::sc_time(0, SC_NS));
  signal_resetn = false;
  sc_start(sc_core::sc_time(1, SC_NS));
  signal_resetn = true;
  sc_start(sc_core::sc_time(9, SC_NS));
  signal_resetn = true;



  return EXIT_FAILURE;
}

// fake sc_man to catch exceptions
int sc_main(int argc, char *argv[])
{
  std::cout << "Hello" << std::endl;
  try {
    return _main(argc, argv);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "Unknown exception occured" << std::endl;
    throw;
  }
  return 1;
}

