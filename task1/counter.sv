module counter #(
    parameter WIDTH = 8                //8-bits
)
(
    input logic clk,                   //clock
    input logic rst,                 //reset
    input logic en,                    //counter enable
    output logic [WIDTH-1:0] count     //count output
);

always_ff @(posedge clk)     //specifies a clocked circuit, only run on rising edge
    if (rst) count <= {WIDTH{1'b0}};
    else count <= count + {{WIDTH-1{1'b0}}, en};    //ensures bit-width matching (no compiler warnings)

endmodule

//{{WIDTH-1{1'b0}}, en} (7 zeros followed by the value of en) --> if en=1, then b00000001
//count <= count + en: adds 1 when en=1, holds when en=0; wraps from ff to 00 on overflow
//counts added on the next rising edge if rst=0 and en=1, resets to 0 if rst is high
//blocking (=) executes sequentially and immediately
//non-blocking (<=) schedule updates to occur at the end of the time step, allow parallel execution