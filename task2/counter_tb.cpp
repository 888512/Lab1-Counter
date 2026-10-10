#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

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

    // init Vbuddy: Open and initialise Vbuddy connection where the port path is in vbuddy.cfg
    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("Lab 1: Counter");

    // initialise simulation inputs, top for top-level entity (only top-level signals visible)
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    // run simulation for many clock cycles
    for (i = 0; i < 100000; i++)
    {

        // dump variables into VCD file and toggle clock
        // output the trace for each half clock cycle --> force model to evaluate both edges of the clock
        for (clk = 0; clk < 2; clk++)
        {
            tfp->dump(2 * i + clk); // unit in ps
            top->clk = !top->clk;
            top->eval();
        }

        // send count value to Vbuddy
        /*
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8) & 0xF);
        vbdHex(2, (int(top->count) >> 4));
        vbdHex(1, int(top->count) & 0xF);
        */

        // plotting: expected a graph where the diagonal lines (0-255) then sudden drop from 255 to 0 and loops
        vbdPlot(int(top->count), 0, 255);

        // change rst and en signals during simulation
        // rst false from i = 2 until 15, iteration i=2 dumps at 4ps and 5ps, rst=0 at the end of the iteration which is 6ps
        top->rst = (i < 2) | (i == 15);
        // en true when i=5 (10ps and 11ps) so end of i=5 en=1 when 12ps
        top->en = vbdFlag();
        if (Verilated::gotFinish())
            exit(0);
    }
    vbdClose();
    tfp->close();
    exit(0);
    // count goes from 00 to 01 in the next rising clock cycle (14ps) after en=1 and rst=0
}