#include <systemc.h>
#include "increment_hard.h"

static float a[4][4] =
   { { 0,0,0,0,} ,
     { 0.5,0,0,0 } ,
     { 0,0,0,0 } ,
     { 0,0,0,0 } } ;

int sc_main(int argc,char *argv[])
{ int X=16,Y=16;



    //P1(X,Y) and S1(X0,Y0)
    float c10 = 6*a[3][0];
    float c01 = 2 * a[2][1];
    float c00 = 6 * a[3][0] + 2 * a[2][0];
    float P1 = c10 * X + c01 * Y + c00;
    //P2(X,Y) and R2(X0,Y0)
    float b20 = 3*a[3][0];
    float b11 = 2*a[2][1];
    float b02 = a[1][2];
    float b10 = 3 * a[3][0] + 2 * a[2][0];
    float b01 = a[2][1] + a[1][1];
    float b00 = a[3][0] + a[2][0] + a[1][0];
    float P2 = b20 * X * X + b11 * X * Y + b02 * Y * Y +
	       b10 * X + b01 * Y +
	       b00 ;
    //P3(X,Y) and Q3(X0,0)
    float P3 = a[3][0] * X * X * X + a[2][1] * X * X * Y + a[1][2] * X * Y * Y + a[0][3] * Y * Y * Y +
	       a[2][0] * X * X + a[1][1] * X * Y + a[0][2] * Y * Y +
	       a[1][0] * X + a[0][1] * Y +
	       a[0][0] ;


    float P0 = 6 * a[3][0];

    float Q0 = 6 * a[0][3];
    float Q1 = 6 * a[0][3] + 2 * a[0][2] +2 * a[1][2] * X + 6 * a[0][3] * Y ;
    float Q2 = a[0][3] + a[0][2]+ a[0][1] + a[2][1] * X * X + 2 * a[1][2] * X * Y 
	       + 3* a[0][3] * Y * Y + (a[1][2] + a[1][1] ) * X + ( 3 * a[0][3] + 2 * a[0][2] ) * Y ;
    float Q3 = P3 ;

    float R0 = 2* a[1][2] ;
    float R1 = a[1][2] + a[2][1] + a[1][1] + 2 * a[1][2] * X + 2 * a[1][2] * Y ;
    float R2 = P2;

    float S0 = 2* a[2][1] ;
    float S1 = P1;


    sc_time     clk_periode(10, SC_NS); // clk period
    sc_clock	signal_clk("signal_clk",clk_periode);
    sc_signal<bool> signal_resetn("signal_resetn");
    sc_signal<bool> signal_init_mux_switch;
    sc_signal<float> p_Q0;
    sc_signal<float> p_Q1;
    sc_signal<float> p_Q2;
    sc_signal<float> p_Q3;
    sc_signal<float> p_R0;
    sc_signal<float> p_R1;
    sc_signal<float> p_R2;
    sc_signal<float> p_S0;
    sc_signal<float> p_S1;
    sc_signal<float> p_P0;
    sc_signal<float> p_OUT;

    soclib::caba::IncrementHard IncrementHard("increment_hard");

    IncrementHard.p_clk(signal_clk);
    IncrementHard.p_resetn(signal_resetn);
    IncrementHard.p_init_mux_switch(signal_init_mux_switch);
    IncrementHard.p_Q0(p_Q0);
    IncrementHard.p_Q1(p_Q1);
    IncrementHard.p_Q2(p_Q2);
    IncrementHard.p_Q3(p_Q3);
    IncrementHard.p_R0(p_R0);
    IncrementHard.p_R1(p_R1);
    IncrementHard.p_R2(p_R2);
    IncrementHard.p_S0(p_S0);
    IncrementHard.p_S1(p_S1);
    IncrementHard.p_P0(p_P0);
    IncrementHard.p_OUT(p_OUT);

    sc_trace_file *TRACEFILE;
    TRACEFILE = sc_create_vcd_trace_file("increment_testbench_traces");
    sc_trace(TRACEFILE, signal_clk,"clk");
    sc_trace(TRACEFILE, signal_resetn,"reset_n");
    sc_trace(TRACEFILE, signal_init_mux_switch,"mux_switch");
    sc_trace(TRACEFILE, p_Q0,"Q0");
    sc_trace(TRACEFILE, p_Q1,"Q1");
    sc_trace(TRACEFILE, p_Q2,"Q2");
    sc_trace(TRACEFILE, p_Q3,"Q3");
    sc_trace(TRACEFILE, p_R0,"R0");
    sc_trace(TRACEFILE, p_R1,"R1");
    sc_trace(TRACEFILE, p_R2,"R2");
    sc_trace(TRACEFILE, p_S0,"S0");
    sc_trace(TRACEFILE, p_S1,"S1");
    sc_trace(TRACEFILE, p_P0,"P0");
    sc_trace(TRACEFILE, p_OUT,"out");

    // reset
    sc_start(sc_core::sc_time(0, SC_NS));
    signal_resetn = false;
    sc_start(sc_core::sc_time(1, SC_NS));
    signal_resetn = true;
    sc_start(sc_core::sc_time(10, SC_NS));
    signal_init_mux_switch = 1;
    p_Q0 = Q0;
    p_Q1 = Q1;
    p_Q2 = Q2;
    p_Q3 = Q3;
    p_R0 = R0;
    p_R1 = R1;
    p_R2 = R2;
    p_S0 = S0;
    p_S1 = S1;
    p_P0 = P0;
    sc_start(sc_core::sc_time(20, SC_NS));
    signal_init_mux_switch = 1;
    sc_start(sc_core::sc_time(1, SC_NS));
    signal_init_mux_switch = 0;
    sc_start(sc_core::sc_time(3000,SC_NS));

    sc_close_vcd_trace_file (TRACEFILE);


    return EXIT_SUCCESS;
}
