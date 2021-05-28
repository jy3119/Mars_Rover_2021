
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

module esp32_0(

	//////////// CLOCK //////////
	input 		          		ADC_CLK_10,
	input 		          		MAX10_CLK1_50,
	input 		          		MAX10_CLK2_50,

	//////////// SDRAM //////////
	output		    [12:0]		DRAM_ADDR,
	output		     [1:0]		DRAM_BA,
	output		          		DRAM_CAS_N,
	output		          		DRAM_CKE,
	output		          		DRAM_CLK,
	output		          		DRAM_CS_N,
	inout 		    [15:0]		DRAM_DQ,
	output		          		DRAM_LDQM,
	output		          		DRAM_RAS_N,
	output		          		DRAM_UDQM,
	output		          		DRAM_WE_N,

	//////////// SEG7 //////////
	output		     [7:0]		HEX0,
	output		     [7:0]		HEX1,
	output		     [7:0]		HEX2,
	output		     [7:0]		HEX3,
	output		     [7:0]		HEX4,
	output		     [7:0]		HEX5,

	//////////// KEY //////////
	input 		     [1:0]		KEY,

	//////////// LED //////////
	output		     [9:0]		LEDR,

	//////////// SW //////////
	input 		     [9:0]		SW,

	//////////// VGA //////////
	output		     [3:0]		VGA_B,
	output		     [3:0]		VGA_G,
	output		          		VGA_HS,
	output		     [3:0]		VGA_R,
	output		          		VGA_VS,

	//////////// Accelerometer //////////
	output		          		GSENSOR_CS_N,
	input 		     [2:1]		GSENSOR_INT,
	output		          		GSENSOR_SCLK,
	inout 		          		GSENSOR_SDI,
	inout 		          		GSENSOR_SDO,

	//////////// Arduino //////////
	inout 		    [15:0]		ARDUINO_IO,
	inout 		          		ARDUINO_RESET_N,

	//////////// GPIO, GPIO connect to GPIO Default //////////
	inout 		    [35:0]		GPIO
);



//=======================================================
//  REG/WIRE declarations
//=======================================================
	wire sda_in;
	wire scl_in;
	wire sda_out;
	wire scl_out;

//=======================================================
//  Structural coding
//=======================================================
	
	nios2_cpu u0 (
		.clk_clk                                                           (MAX10_CLK1_50),                                                           //               clk.clk
		.reset_reset_n                                                     (1'b1),                                                     //             reset.reset_n
		.i2c_port_conduit_end_conduit_data_in (sda_in),  // i2c_port_conduit_end.conduit_data_in
		.i2c_port_conduit_end_conduit_clk_in  (scl_in),  //                     .conduit_clk_in
		.i2c_port_conduit_end_conduit_data_oe (sda_out),  //                     .conduit_data_oe
		.i2c_port_conduit_end_conduit_clk_oe  (scl_out)   //              .sclk_to_the_spislave_inst_for_spichain
	);

	i2c_adaptor a0 (
		.ad_sda(ARDUINO_IO[4]),			// GPIO21 ON ESP32
		.ad_scl(ARDUINO_IO[3]), 	   // GPIO22 ON ESP32
		.ad_sda_in(sda_in),
		.ad_scl_in(scl_in),
		.ad_sda_oe(sda_out),
		.ad_scl_oe(scl_out)
	);


endmodule
