
// C/C++ std libs
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <systemc>

#include "increment_hard.h"
#include "utils.h"

using namespace soclib::caba;
using namespace sc_core;

int _main(int argc, char* argv[]) {

  sc_time     clk_periode(10, SC_NS); // clk period
  sc_clock	signal_clk("signal_clk",clk_periode);
  sc_signal<bool> signal_resetn("signal_resetn");

  sc_signal<bool>              signal_init_mux_switch;
  sc_signal<float>             signal_Q0;
  sc_signal<float>             signal_Q1;
  sc_signal<float>             signal_Q2;
  sc_signal<float>             signal_Q3;
  sc_signal<float>             signal_R0;
  sc_signal<float>             signal_R1;
  sc_signal<float>             signal_R2;
  sc_signal<float>             signal_S0;
  sc_signal<float>             signal_S1;
  sc_signal<float>             signal_P0;
  sc_signal<float>            signal_OUT;

  IncrementHard incrementHard("increment_hard");
  incrementHard.p_clk(signal_clk);
  incrementHard.p_resetn(signal_resetn);
  incrementHard.p_init_mux_switch(signal_init_mux_switch);
  incrementHard.p_Q0(signal_Q0);
  incrementHard.p_Q1(signal_Q1);
  incrementHard.p_Q2(signal_Q2);
  incrementHard.p_Q3(signal_Q3);
  incrementHard.p_R0(signal_R0);
  incrementHard.p_R1(signal_R1);
  incrementHard.p_R2(signal_R2);
  incrementHard.p_S0(signal_S0);
  incrementHard.p_S1(signal_S1);
  incrementHard.p_P0(signal_P0);
  incrementHard.p_OUT(signal_OUT);

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

