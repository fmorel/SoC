
// C/C++ std libs
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <systemc>

#include "utils.h"
#include "increment.h"

using namespace soclib::caba;
using namespace sc_core;

int _main(int argc, char* argv[]) {

  // Define our WB parameters
  typedef soclib::caba::WbParams<32,32> wb_param;

  sc_time     clk_periode(10, SC_NS); // clk period
  sc_clock	signal_clk("signal_clk",clk_periode);
  sc_signal<bool> signal_resetn("signal_resetn");

  Increment<wb_param> increment("Increment");

  soclib::caba::WbSignal<wb_param> signal_wb_slave("signal_wb_slave");
  soclib::caba::WbSignal<wb_param> signal_wb_master("signal_wb_master");

  increment.p_clk(signal_clk);
  increment.p_resetn(signal_resetn);
  //wishbone interface
  increment.p_wb_master(signal_wb_master);
  increment.p_wb_slave(signal_wb_slave);
 

  //TODO
  //Est-ce qu'on veut tester ce module indépendant du wishbone ?
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

