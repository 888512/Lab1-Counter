module counter #(
    parameter WIDTH = 8                //8-bits
)
(
    input logic clk,                   //clock
    input logic rst_n,                 //reset
    input logic en,                    //counter enable
    output logic [WIDTH-1:0] count     //count output
);

always_ff @(posedge clk)     //specifies a clocked circuit
    if (rst_n == 1'b0) count <= {WIDTH{1'b0}};
    else count <= count + {{WIDTH-1{1'b0}}, en};    //ensures bit-width matching (no compiler warnings)

endmodule

//{{WIDTH-1{1'b0}}, en} = b00000001 (7 zeros and 1 one --> if en is high)
//count += 1 (with bit-width matching)
//counts on previous edge of clock if en is high, resets to 0 if rst is high
