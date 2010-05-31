
// C/C++ std libs
#include <iostream>
#include <cstdlib>
#include <cstdio>

#include <systemc>


#include "buffer.h"
#include "utils.h"

using namespace soclib::caba;
using namespace sc_core;

int _main(int argc, char* argv[]) {

  int test_failed=0;

  sc_time     clk_periode(10, SC_NS); // clk period
  sc_clock	signal_clk("signal_clk",clk_periode);
  sc_signal<bool> signal_resetn("signal_resetn");


  //Reading the buffer
  sc_signal<unsigned char>    buffer_out[4];
  sc_signal<uint32_t>               buffer_command_out[2];

  //Writing to the buffer
  sc_signal<uint32_t>           buffer_in;
  sc_signal<bool>      buffer_write_enable;


  Buffer buffer("buffer");


  buffer.p_clk(signal_clk);
  buffer.p_resetn(signal_resetn);
  buffer.buffer_in(buffer_in);
  buffer.buffer_write_enable(buffer_write_enable);
  for (int i = 0; i < 4;i++) {
    buffer.buffer_out[i](buffer_out[i]);
  }
  for (int i = 0; i < 2;i++) {
    buffer.buffer_command_out[i](buffer_command_out[i]);
  }


  sc_start(sc_core::sc_time(0, SC_NS));
  signal_resetn = false;
  sc_start(sc_core::sc_time(1, SC_NS));
  signal_resetn = true;
  sc_start(sc_core::sc_time(9, SC_NS));
  signal_resetn = true;

  std::cout << "Let's go..." << std::endl;

  //write 2 tiles
  for (int k = 1;k<3;k++) {
  buffer_write_enable = true;
  for( int i=0;i<ANTE_TILE_SIZE;i++) {
    for (int j = 0; j<ANTE_TILE_SIZE/4;j++) {
      buffer_in = (((i+j*4) *k) << 24) + (((i+j*4+1) *k) << 16) + (((i+j*4+2) *k) << 8) + (((i+j*4+3) *k) << 0);
      sc_start(sc_time(10,SC_NS));
    }
  }
  buffer_write_enable = false;
  }

  //Read the first tile
  buffer_command_out[0] = 4;
  buffer_command_out[1] = 4;
  sc_start(sc_time(10,SC_NS));
  if (buffer_out[0].read() != buffer_command_out[0].read() + buffer_command_out[1].read()){
    printf("TEST FAILED !\n");
    test_failed++;
  }
  if (buffer_out[1].read() != buffer_command_out[0].read() + buffer_command_out[1].read()+1){
    printf("TEST FAILED !\n");
    test_failed++;
  }
  if (buffer_out[2].read() != buffer_command_out[0].read() + buffer_command_out[1].read()+1){
    printf("TEST FAILED !\n");
    test_failed++;
  }
  if (buffer_out[3].read() != buffer_command_out[0].read() + buffer_command_out[1].read()+1+1){
    printf("TEST FAILED !\n");
    test_failed++;
  }

  //Write one pixel to trigger tile switch, and read from the other tile.
  buffer_write_enable = true;
  buffer_in = 0x12345678;
  sc_start(sc_time(10,SC_NS));

  if (buffer_out[0].read() != (buffer_command_out[0].read() + buffer_command_out[1].read())*2){
    printf("TEST FAILED !\n");
    test_failed++;
  }
  if (buffer_out[1].read() != (buffer_command_out[0].read() + buffer_command_out[1].read()+1)*2){
    printf("TEST FAILED !\n");
    test_failed++;
  }
  if (buffer_out[2].read() != (buffer_command_out[0].read() + buffer_command_out[1].read()+1)*2){
    printf("TEST FAILED !\n");
    test_failed++;
  }
  if (buffer_out[3].read() != (buffer_command_out[0].read() + buffer_command_out[1].read()+1+1)*2){
    printf("TEST FAILED !\n");
    test_failed++;
  }
  

  if (test_failed==0) {
    printf("TEST PASSED\n");
  }
  return EXIT_SUCCESS;
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

