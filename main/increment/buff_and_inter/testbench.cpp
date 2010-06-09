/*
 *
 * SOCLIB_LGPL_HEADER_BEGIN
 *
 * This file is part of SoCLib, GNU LGPLv2.1.
 *
 * SoCLib is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; version 2.1 of the License.
 *
 * SoCLib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SoCLib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * SOCLIB_LGPL_HEADER_END
 *
 */

//Include this first because of #define conflict (#define fp 27 somwhere screws up sdl header)
//#include "display.h"
#define DO_TRACES

// C/C++ std libs
#include <iostream>
#include <cstdlib>
#include <cstdio>

// SocLib
#include "mapping_table.h"
#include "vci_ram.h"
#include "vci_rom.h"

#include "wb_signal.h"


// WB interconnect
#include "wb_interco.h"
// WB->VCI wrappers
#include "wb_slave_vci_initiator_wrapper.h"

// locals
#include "segmentation.h"
#include "buff_and_inter.h"

//our module
#include "buff_and_inter.h"


// real SystemC main
int _main(int argc, char *argv[])
{
  using namespace sc_core;
  // Avoid repeating these everywhere
  using soclib::common::IntTab;
  using soclib::common::Segment;

  // Define our VCI parameters
  typedef soclib::caba::VciParams<4,8,32,1,1,1,8,1,1,1> vci_param;

  // Define our WB parameters
  typedef soclib::caba::WbParams<32,32> wb_param;

  // Mapping table
  soclib::common::MappingTable maptab(32, IntTab(8), IntTab(8), 0x80000000);

  maptab.add(Segment("rom" , ROM_BASE , ROM_SIZE , IntTab(0), true));
  maptab.add(Segment("ram" , RAM_BASE , RAM_SIZE , IntTab(1), true));
  //add simple slave
  maptab.add(Segment("buffer" , BUFFER_BASE , BUFFER_SIZE , IntTab(2), false));

  // Gloabal signals
  sc_time     clk_periode(10, SC_NS); // clk period
  //sc_time	video_clk_periode(40,SC_NS);	//video clk at 25MHz
  sc_clock	signal_clk("signal_clk",clk_periode);
  //sc_clock 	signal_video_clk("signal_video_clk",video_clk_periode);

  sc_signal<bool> signal_resetn("signal_resetn");

  /****************************************
   ************ BufInter ports ************
   ****************************************/
  ////// data ports ///////
  //x and y used during interpolation phase
  sc_signal<float>   buff_inter_x;
  sc_signal<float>   buff_inter_y;
  //x_min and y_min used during the buff_inter_buffering  phase
  sc_signal<float>   buff_inter_x_min;
  sc_signal<float>   buff_inter_y_min;
  //main output
  sc_signal<char>    buff_inter_intensity;

  //////  control ports /////
  sc_signal<bool>    buff_inter_new_tile;
  sc_signal<bool>    buff_inter_ask_for_x_y;
  sc_signal<bool>    buff_inter_valid;

  //WB master
  soclib::caba::WbSignal<wb_param> buff_inter_master("signal_buffer_master");

  //WB slave
  soclib::caba::WbSignal<wb_param> buff_inter_slave("signal_buffer_master");



  // interconnection signals
  soclib::caba::VciSignals<vci_param> signal_vci_rom("signal_vci_vcirom");
  soclib::caba::VciSignals<vci_param> signal_vci_ram("signal_vci_vciram");

  // WB interconnect signals
  soclib::caba::WbSignal<wb_param> signal_wb_ram("signal_wb_ram");
  soclib::caba::WbSignal<wb_param> signal_wb_rom("signal_wb_rom");





  ////////////////////////////////////////////////////////////
  /////////////////////// Components /////////////////////////
  ////////////////////////////////////////////////////////////


  // memories
  soclib::caba::VciRom<vci_param> rom("rom", IntTab(0), maptab);
  soclib::caba::VciRam<vci_param> ram("ram", IntTab(1), maptab);

  // WB interconnect
  //                                           sc_name    maptab  masters slaves
  soclib::caba::WbInterco<wb_param> wbinterco("wbinterco",maptab, 1,3);

  //VideoGen
  //soclib::caba::VideoGen my_videogen ("video_gen");

  //buff_inter
  soclib::caba::BufInter<wb_param> buff_inter("buff_inter");
  buff_inter.p_clk (signal_clk);
  buff_inter.p_resetn (signal_resetn);
  buff_inter.p_wb_slave (buff_inter_slave);
  buff_inter.p_wb_master (buff_inter_master);
  buff_inter.x(buff_inter_x);
  buff_inter.y(buff_inter_y);
  buff_inter.x_min(buff_inter_x_min);
  buff_inter.y_min(buff_inter_y_min);
  buff_inter.intensity(buff_inter_intensity);
  buff_inter.new_tile(buff_inter_new_tile);
  buff_inter.ask_for_x_y(buff_inter_ask_for_x_y);
  buff_inter.valid(buff_inter_valid);
  ////////////////////////////////////////////////////////////
  /////////////////// WB -> VCI Wrappers /////////////////////
  ////////////////////////////////////////////////////////////

  // ram
  soclib::caba::WbSlaveVciInitiatorWrapper<vci_param, wb_param> ram_w ("ram_w") ;
  ram_w.p_clk               (signal_clk);
  ram_w.p_resetn            (signal_resetn);
  ram_w.p_vci               (signal_vci_ram);
  ram_w.p_wb                (signal_wb_ram);

  // rom
  soclib::caba::WbSlaveVciInitiatorWrapper<vci_param, wb_param> rom_w ("rom_w") ;
  rom_w.p_clk               (signal_clk);
  rom_w.p_resetn            (signal_resetn);
  rom_w.p_vci               (signal_vci_rom);
  rom_w.p_wb                (signal_wb_rom);


  ///////////////////// WB Net List //////////////////////////
  ////////////////////////////////////////////////////////////

  wbinterco.p_clk(signal_clk);
  wbinterco.p_resetn(signal_resetn);

  wbinterco.p_from_master[0](buff_inter_master);
  wbinterco.p_to_slave[0](signal_wb_rom);
  wbinterco.p_to_slave[1](signal_wb_ram);
  wbinterco.p_to_slave[2](buff_inter_slave);

  ////////////////////////////////////////////////////////////
  //////////////////// VCI Net List //////////////////////////
  ////////////////////////////////////////////////////////////

  // rom
  rom.p_clk(signal_clk);
  rom.p_resetn(signal_resetn);
  rom.p_vci(signal_vci_rom);

  // ram
  ram.p_clk(signal_clk);
  ram.p_resetn(signal_resetn);
  ram.p_vci(signal_vci_ram);


  ////////////////////////////////////////////////////////////
  //////////////// Traces ////////////////////////////////////
  ////////////////////////////////////////////////////////////

#ifdef DO_TRACES
  sc_trace_file *TRACEFILE;
  TRACEFILE = sc_create_vcd_trace_file("vcd_traces");
  //sc_trace (TRACEFILE, signal_resetn, "resetn" );
  //sc_trace (TRACEFILE, signal_clk,    "clk"    );
  sc_trace (TRACEFILE, buff_inter_master ,"master");
  sc_trace (TRACEFILE, buff_inter_x_min ,"x_min");
  sc_trace (TRACEFILE, buff_inter_y_min ,"y_min");
  sc_trace (TRACEFILE, buff_inter_valid ,"valid");
  sc_trace (TRACEFILE, buff_inter_ask_for_x_y ,"ask");
  sc_trace (TRACEFILE, buff_inter_new_tile ,"new_tile");

#endif

  ////////////////////////////////////////////////////////////
  //////////////// Start Simulation //////////////////////////
  ////////////////////////////////////////////////////////////

  // reset
  sc_start(sc_core::sc_time(0, SC_NS));
  signal_resetn = false;
  sc_start(sc_core::sc_time(1, SC_NS));
  signal_resetn = true;

  // print maping table
  std::cout << "Let's go..." << std::endl;
  std::cout << maptab;

  // run SystemC simulator
  buff_inter_x_min=10.5;
  buff_inter_y_min=49.245;
  // sc_start(sc_core::sc_time(1000, SC_NS));
  sc_start( );

#ifdef DO_TRACES
  sc_close_vcd_trace_file(TRACEFILE);
#endif

  return EXIT_SUCCESS;
}

// fake sc_man to catch exceptions
int sc_main(int argc, char *argv[])
{
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

