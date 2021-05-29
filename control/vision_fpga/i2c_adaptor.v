module i2c_adaptor(
    inout tri1 ad_scl,
    inout tri1 ad_sda,

    output wire ad_sda_in,
    output wire ad_scl_in,
    input wire ad_sda_oe,
    input wire ad_scl_oe
);

    assign ad_sda_in = ad_sda;
    assign ad_scl_in = ad_scl;
    assign ad_sda = ad_sda_oe ? 1'b0 : 1'bz;
    assign ad_scl = ad_scl_oe ? 1'b0 : 1'bz;

endmodule