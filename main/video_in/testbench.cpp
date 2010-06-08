
// C/C++ std libs
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <systemc>

#include "utils.h"
#include "video_in.h"

using namespace soclib::caba;
using namespace sc_core;

int _main(int argc, char* argv[]) {

  // Define our WB parameters
  typedef soclib::caba::WbParams<32,32> wb_param;

  VideoIn<wb_param> videoIN("Video_in");

  sc_time     clk_periode(10, SC_NS); // clk period
  sc_clock	signal_clk("signal_clk",clk_periode);
  sc_time     video_clk_periode(40, SC_NS); // clk period
  sc_clock	signal_video_clk("signal_video_clk",video_clk_periode);
  sc_signal<bool> signal_resetn("signal_resetn");

  sc_signal<bool>             signal_line_valid;
  sc_signal<bool>             signal_frame_valid;
  sc_signal<unsigned char>    signal_pixel_in;

  sc_signal<bool>            signal_interrupt;

  soclib::caba::WbSignal<wb_param> signal_wb_video_in_slave("signal_wb_video_in_slave");
  soclib::caba::WbSignal<wb_param> signal_wb_video_in_master("signal_wb_video_in_master");

  videoIN.p_clk(signal_clk);
  videoIN.p_video_clk(signal_video_clk);
  videoIN.p_resetn(signal_resetn);

  videoIN.line_valid(signal_line_valid);
  videoIN.frame_valid(signal_frame_valid);
  videoIN.pixel_in(signal_pixel_in);

  videoIN.interrupt(signal_interrupt);
  videoIN.p_wb_master(signal_wb_video_in_master);
  videoIN.p_wb_slave(signal_wb_video_in_slave);

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

