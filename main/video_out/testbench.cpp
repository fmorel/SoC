
// C/C++ std libs
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <systemc>

#include "utils.h"
#include "video_out.h"

using namespace soclib::caba;
using namespace sc_core;

int _main(int argc, char* argv[]) {
  
  typedef soclib::caba::WbParams<32,32> wb_param;
  VideoOut<wb_param> videoOut("Video_out");

  sc_time     clk_periode(10, SC_NS); // clk period
  sc_clock	signal_clk("signal_clk",clk_periode);
  sc_time     video_clk_periode(40, SC_NS); // clk period
  sc_clock	signal_video_clk("signal_video_clk",video_clk_periode);
  sc_signal<bool> signal_resetn("signal_resetn");

  sc_signal<bool>             signal_line_valid;
  sc_signal<bool>             signal_frame_valid;
  sc_signal<unsigned char>    signal_pixel_out;

  sc_signal<bool>            signal_interrupt;

  soclib::caba::WbSignal<wb_param> signal_wb_video_out_slave("signal_wb_video_out_slave");
  soclib::caba::WbSignal<wb_param> signal_wb_video_out_master("signal_wb_video_out_master");

  videoOut.p_clk(signal_clk);
  videoOut.p_video_clk(signal_video_clk);
  videoOut.p_resetn(signal_resetn);

  videoOut.p_line_valid(signal_line_valid);
  videoOut.p_frame_valid(signal_frame_valid);
  videoOut.p_pixel_out(signal_pixel_out);

  videoOut.p_interrupt(signal_interrupt);
  videoOut.p_wb_master(signal_wb_video_out_master);
  videoOut.p_wb_slave(signal_wb_video_out_slave);

  //TODO
  //Est-ce qu'on veut tester ce module indépendant du wishbone ?

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

