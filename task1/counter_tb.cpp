#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env)
{
    // i count the number of clock cycles to simulate
    // clk is the module clock signal
    int i;
    int clk;

    // init top verilog instance
    Verilated::commandArgs(argc, argv);
    // enable tracing globally
    Verilated::traceEverOn(true);
    // init trace dump
    Vcounter *top = new Vcounter;
    // DUT part (not mandatory if don't want to see the waveforms)
    // create the trace writer, where VerilatedVcdC writes a Value Change Dump(VCD) file --> turns simulation into a waveform
    VerilatedVcdC *tfp = new VerilatedVcdC;
    // attach it to the Device Under Test(DUT)
    top->trace(tfp, 99);
    // open the output file
    tfp->open("counter.vcd");

    // initialise simulation inputs, top for top-level entity (only top-level signals visible)
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for (i = 0; i < 300; i++)
    {

        // dump variables into VCD file and toggle clock
        // output the trace for each half clock cycle --> force model to evaluate both edges of the clock
        for (clk = 0; clk < 2; clk++)
        {
            tfp->dump(2 * i + clk); // unit in ps
            top->clk = !top->clk;
            top->eval();
        }
        // change rst and en signals during simulation
        // rst false from i = 2 until 15, iteration i=2 dumps at 4ps and 5ps, rst=0 at the end of the iteration which is 6ps
        top->rst = (i < 2) | (i == 15);
        // en true when i=5 (10ps and 11ps) so end of i=5 en=1 when 12ps
        top->en = (i > 4);
        if (Verilated::gotFinish())
            exit(0);
    }
    tfp->close();
    exit(0);
    // count goes from 00 to 01 in the next rising clock cycle (14ps) after en=1 and rst=0
}