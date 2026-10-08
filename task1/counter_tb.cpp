#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env)
{
    int i;
    int clk;
    // i count the number of clock cycles to simulate
    // clk is the module clock signal

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter *top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    // enable tracing globally
    VerilatedVcdC *tfp = new VerilatedVcdC;
    // DUT part (not mandatory if don't want to see the waveforms)
    //  create the trace writer, where VerilatedVcdC writes a Value Change Dump(VCD) file --> turns simulation into a waveform
    top->trace(tfp, 99);
    // attach it to the Device Under Test(DUT)
    tfp->open("counter.vcd");
    // open the output file

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
            tfp->dump(2 * i * clk); // unit in ps
            top->clk = !top->clk;
            top->eval();
        }
        // change rst and en signals during simulation
        top->rst = (i < 2) | (i == 15);
        top->en = (i > 4);
        if (Verilated::gotFinish())
            exit(0);
    }
    tfp->close();
    exit(0);
}