
// C/C++ std libs
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <systemc>

#include "utils.h"
#include "interpolation.h"

using namespace soclib::caba;
using namespace sc_core;
using namespace std;

int _main(int argc, char* argv[]) {

  sc_trace_file *TRACEFILE;
  TRACEFILE = sc_create_vcd_trace_file("vcd_traces");
   int test_failed = 0;

  Interpolation interpolation("interpolation");

  sc_time     clk_periode(10, SC_NS); // clk period
  sc_clock	signal_clk("signal_clk",clk_periode);
  sc_signal<bool> signal_resetn("signal_resetn");

  sc_signal<float>             signal_x;
  sc_signal<float>             signal_y;
  sc_signal<unsigned char>     signal_out;

  sc_signal<unsigned char>     signal_buffer_in[4];
  sc_signal<int>          signal_buffer_command[2];

  interpolation.p_clk(signal_clk);
  interpolation.p_resetn(signal_resetn);

  interpolation.p_x(signal_x);
  interpolation.p_y(signal_y);

  interpolation.p_out(signal_out);

  for (int i = 0;i<4;i++) {
    interpolation.p_buffer_in[i](signal_buffer_in[i]);
  }
  for (int i = 0;i<2;i++) {
    interpolation.p_buffer_command[i](signal_buffer_command[i]);
  }
  sc_trace (TRACEFILE, signal_clk,    "clk"    );
  sc_trace (TRACEFILE, signal_resetn,    "resetn"    );
  sc_trace (TRACEFILE, signal_out,    "out"    );
  sc_trace (TRACEFILE, signal_x,    "x"    );
  sc_trace (TRACEFILE, signal_y,    "y"    );
  sc_trace (TRACEFILE, signal_buffer_in[0], "bufferin0");
  sc_trace (TRACEFILE, signal_buffer_in[1], "bufferin1");
  sc_trace (TRACEFILE, signal_buffer_in[2], "bufferin2");
  sc_trace (TRACEFILE, signal_buffer_in[3], "bufferin3");
  sc_trace (TRACEFILE, signal_buffer_command[0], "buffercommand0");
  sc_trace (TRACEFILE, signal_buffer_command[1], "buffercommand1");


  sc_start(sc_core::sc_time(0, SC_NS));
  signal_resetn = false;
  sc_start(sc_core::sc_time(1, SC_NS));
  signal_resetn = true;
  sc_start(sc_core::sc_time(9, SC_NS));



  cout << "Begining tests" << endl;
  
  signal_x = 1.0;
  signal_y = 1.0;
  sc_start(sc_core::sc_time(10, SC_NS));

  //Check that command_out is okay
  if (1 != signal_buffer_command[0] || 1 != signal_buffer_command[1]) {
    cout << "failed !" << endl;
    test_failed++;
  }

  signal_x = 2.3;
  signal_y = 3.6;

  sc_start(sc_core::sc_time(5, SC_NS));
  //The buffer answers
  for (int i = 0;i<4;i++) {
    signal_buffer_in[i] = 50*i;
  }

  sc_start(sc_core::sc_time(5, SC_NS));
  //Check that command_out is okay
  if (2 != signal_buffer_command[0] || 3 != signal_buffer_command[1]){
    cout << "command_out failed !" << endl;
    test_failed++;
  }
  sc_start(sc_core::sc_time(10, SC_NS));
  //Check that out is okay
  if (0!= signal_out) {
    cout << "out failed !" << endl;
    cout << (float)signal_out.read() << endl ;
    test_failed++;
  }
  
  sc_start(sc_core::sc_time(10, SC_NS));
//Check that out is okay
  int temp = 0.7*0.4*0+0.7*0.6*50+0.3*0.4*100+0.3*0.6*150 ;
  int signal_temp = signal_out ;
  //Weird rounding issues. Check that the difference is less than 2.
  if ((temp - signal_temp) > 2 || (signal_temp - temp > 2) ) {
    cout << "out failed !" << endl;
    cout << signal_temp << endl ;
    cout << temp << endl;
    test_failed++;
  }

  sc_close_vcd_trace_file(TRACEFILE);
  if (test_failed>0) {
    return EXIT_FAILURE;
  }
  else return EXIT_SUCCESS;
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
  }
  return EXIT_FAILURE;
}
