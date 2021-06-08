module EEE_IMGPROC(
	// global clock & reset
	clk,
	reset_n,
	
	// mm slave
	s_chipselect,
	s_read,
	s_write,
	s_readdata,
	s_writedata,
	s_address,

	// stream sink
	sink_data,
	sink_valid,
	sink_ready,
	sink_sop,
	sink_eop,
	
	// streaming source
	source_data,
	source_valid,
	source_ready,
	source_sop,
	source_eop,
	
	// conduit
	mode
	
);


// global clock & reset
input	clk;
input	reset_n;

// mm slave
input							s_chipselect;
input							s_read;
input							s_write;
output	reg	[31:0]	s_readdata;
input	[31:0]				s_writedata;
input	[2:0]					s_address;


// streaming sink
input	[23:0]            	sink_data;
input								sink_valid;
output							sink_ready;
input								sink_sop;
input								sink_eop;

// streaming source
output	[23:0]			  	   source_data;
output								source_valid;
input									source_ready;
output								source_sop;
output								source_eop;

// conduit export
input                         mode;

////////////////////////////////////////////////////////////////////////
//
parameter IMAGE_W = 11'd640;
parameter IMAGE_H = 11'd480;
parameter MESSAGE_BUF_MAX = 256;
parameter MSG_INTERVAL = 6;
parameter BB_COL_DEFAULT = 24'h00ff00;


wire [7:0]   red, green, blue, grey;
wire [7:0]	 magenta, yellow;
wire [7:0]   red_out, green_out, blue_out;

wire         sop, eop, in_valid, out_ready;
////////////////////////////////////////////////////////////////////////

//RGB TO HSV
reg [7:0]	current_r, current_g, current_b;		//to be used for latching
reg [7:0]	hue, saturation, value;		//output values
reg [7:0]	maximum, minimum, difference;			//temp values
reg [7:0] denominator
reg [13:0] denom_mult_60;//*60
reg [7:0] temp_sat;
reg [7:0] temp_max;
reg [7:0] division;//division
reg [1:0] hsv_state


always @(posedge clk) begin
	{current_r, current_g, current_b} <= {red, green, blue}; //latch
	if((current_r >= current_b) && (current_r >= current_g) && (current_b >= current_g)) begin //R_B_G
		maximum <= current_r;
		minimum <= current_g;
		denominator <= current_b - current_g;
		denom_mult_60 <= {denominator,6'b000000} - {denominator,2'b00};
		difference <= maximum - minimum;
	end else if((current_r >= current_b) && (current_r >= current_g) && (current_b >= current_g)) begin //R_G_B
		maximum <= current_r;
		minimum <= current_b;
		denominator <= current_g - current_b;
		denom_mult_60 <= {denominator,6'b000000} - {denominator,2'b00};
		difference <= maximum - minimum;
	end else if((current_g >= current_b) && (current_g >= current_r) && (current_b >= current_r)) begin //G_B_R
		maximum <= current_g;
		minimum <= current_r;
		denominator <= current_b - current_r;
		denom_mult_60 <= {denominator,6'b000000} - {denominator,2'b00};
		difference <= maximum - minimum;
	end else if((current_g >= current_b) && (current_g >= current_r) && (current_r >= current_b)) begin //G_R_B
		maximum <= current_g;
		minimum <= current_b;
		denominator <= current_r - current_b;
		denom_mult_60 <= {denominator,6'b000000} - {denominator,2'b00};
		difference <= maximum - minimum;
	end else if((current_b >= current_r) && (current_b >= current_g) && (current_r >= current_g)) begin //B_R_G
		maximum <= current_b;
		minimum <= current_g;
		denominator <= current_r - current_g;
		denom_mult_60 <= {denominator,6'b000000} - {denominator,2'b00};
		difference <= maximum - minimum;
	end else if((current_b >= current_r) && (current_b >= current_g) && (current_g >= current_r)) begin //B_R_G
		maximum <= current_b;
		minimum <= current_r;
		denominator <= current_g - current_r;
		denom_mult_60 <= {denominator,6'b000000} - {denominator,2'b00};
		difference <= maximum - minimum;
	end
end

//Get Hue
always @(posedge clk) begin
	division = (difference > 8'd0) ? denom_mult_60 / difference : 8'd240;//Note that max = min  
	if((current_r >= current_b) && (current_r >= current_g) && (current_b >= current_g)) hue <= 9'd360 - division; //RBG
	else if((current_r >= current_b) && (current_r >= current_g) && (current_b >= current_g)) hue <= division; //R_G_B
	else if((current_g >= current_b) && (current_g >= current_r) && (current_b >= current_r)) hue <= 9'd120 + division; //G_B_R
	else if((current_g >= current_b) && (current_g >= current_r) && (current_r >= current_b)) hue <= 9'd120 - division; //G_R_B
	else if((current_b >= current_r) && (current_b >= current_g) && (current_r >= current_g)) hue <= 9'd240 + division; //B_R_G
	else if((current_b >= current_r) && (current_b >= current_g) && (current_g >= current_r)) hue <= 9'd240 - division; //B_gr
end

//Get Saturation
always @(posedge clk) begin
	temp_sat = (max > 8'd0)? {difference[7:0],8'b00000000} / max : 8'd0;
	saturation <= temp_sat;
end

//Get Value
always @(posedge clk) begin
	value <= maximum;
end

////////////////////////////////////////////////////////////////

// Detect other colours
assign magenta = red[7:1] + blue[7:1]; //pink = red/2 + blue/2
assign yellow = red[7:1] + green[7:1]; //yellow = red/2 + green/2

// Detect red areas
wire red_detect;
assign red_detect = red[7] & ~green[7] & ~blue[7] & ~magenta[7] & ~yellow[7];

// Detect green areas
wire green_detect;
assign green_detect = ~red[7] & green[7] & ~blue[7] & ~magenta[7] & ~yellow[7];

// Detect blue areas
wire blue_detect;
assign blue_detect = ~red[7] & ~green[7] & blue[7] & ~magenta[7] & ~yellow[7];

//Detect magenta areas
wire magenta_detect;
assign magenta_detect = magenta[7]; //& ~red[7] & ~green[7] & ~blue[7] & ~yellow[7];

//detect yellow areas
wire yellow_detect;
assign yellow_detect = yellow[7]; //~magenta[7] & ~red[7] & ~green[7] & ~blue[7];

//detecting ball in general
wire ball_detect;
assign ball_detect = red_detect | green_detect | blue_detect | yellow_detect | magenta_detect;

// Find boundary of cursor box

// Highlight detected areas
wire [23:0] red_high, green_high, blue_high, magenta_high, yellow_high;
assign grey = green[7:1] + red[7:2] + blue[7:2]; //Grey = green/2 + red/4 + blue/4
assign red_high  =  red_detect ? {8'hff, 8'h0, 8'h0} : {grey, grey, grey};
assign green_high = green_detect ? {8'h0, 8'hff, 8'h0} : {grey, grey, grey};
assign blue_high = blue_detect ? {8'h0, 8'h0, 8'hff} : {grey, grey, grey};

assign magenta_high = magenta_detect ? {8'hff, 8'h0, 8'hff} : {grey, grey, grey};
assign yellow_high = magenta_detect ? {8'hff, 8'hff, 8'h0} : {grey, grey, grey};

// Show bounding box
wire [23:0] new_image;
wire bb_active;
assign bb_active = (x == left) | (x == right) | (y == top) | (y == bottom);
assign new_image = bb_active ? bb_col : red_high | green_high | blue_high | magenta_high | yellow_high;

// Switch output pixels depending on mode switch
// Don't modify the start-of-packet word - it's a packet discriptor
// Don't modify data in non-video packets
assign {red_out, green_out, blue_out} = (mode & ~sop & packet_video) ? new_image : {red,green,blue};

//Count valid pixels to tget the image coordinates. Reset and detect packet type on Start of Packet.
reg [10:0] x, y;
reg packet_video;
always@(posedge clk) begin
	if (sop) begin
		x <= 11'h0;
		y <= 11'h0;
		packet_video <= (blue[3:0] == 3'h0);
	end
	else if (in_valid) begin
		if (x == IMAGE_W-1) begin
			x <= 11'h0;
			y <= y + 11'h1;
		end
		else begin
			x <= x + 11'h1;
		end
	end
end

//Find first and last red pixels
reg [10:0] x_min, y_min, x_max, y_max;
always@(posedge clk) begin
	if (ball_detect & in_valid) begin	//Update bounds when the pixel is red
		if (x < x_min) x_min <= x;
		if (x > x_max) x_max <= x;
		if (y < y_min) y_min <= y;
		y_max <= y;
	end
	if (sop & in_valid) begin	//Reset bounds on start of packet
		x_min <= IMAGE_W-11'h1;
		x_max <= 0;
		y_min <= IMAGE_H-11'h1;
		y_max <= 0;
	end
end

reg [10:0] x_length, y_length, x_center;	
reg [21:0] box_size;	

always@(posedge clk) begin
	if (ball_detect& in_valid) begin
		x_length <= x_max - x_min;
		y_length <= y_max - y_min;
		box_size <= x_length * y_length;
		x_center <= (x_min + x_max) >> 2;
	end
end

reg[10:0] dist_from_mid;
reg[15:0] distance;
reg[7:0] angle, detected;

always@(posedge clk) begin
	detected <= 8'b0;
	if (box_size > 21'b0000001010011101011000) begin	//value when it is 25cm away -> 1m away after simulation is possible
		detected <= 8'b1;
		distance <= 1071000 / box_size; 						//agar agar first but the division needs to be fixed.
			if (x_center > 11'b00101000000) begin
				dist_from_mid <= 11'b00101000000 - x_center;
				angle <= 8'b0;
				//$acos(distance, dist_from_mid);		//not sure if acos is gna work
			end else if (x_center < 11'b00101000000) begin
				dist_from_mid <= x_center - 11'b00101000000;
				angle <= 8'b1; //-1 * $acos(distance, dist_from_mid); // negative
			end
	end
end

//Process bounding box at the end of the frame.
reg [1:0] msg_state;
reg [10:0] left, right, top, bottom;
reg[15:0] dist_;
reg[7:0] ang_, det_;
reg [7:0] frame_count;
always@(posedge clk) begin
	if (eop & in_valid & packet_video) begin  //Ignore non-video packets
		
		//Latch edges for display overlay on next frame
		left <= x_min;
		right <= x_max;
		top <= y_min;
		bottom <= y_max;
		
		dist_ <= distance;
		ang_ <= angle;
		det_ <= detected;	
		
		
		//Start message writer FSM once every MSG_INTERVAL frames, if there is room in the FIFO
		frame_count <= frame_count - 1;
		
		if (frame_count == 0 && msg_buf_size < MESSAGE_BUF_MAX - 3) begin
			msg_state <= 2'b01;
			frame_count <= MSG_INTERVAL-1;
		end
	end
	
	//Cycle through message writer states once started
	if (msg_state != 2'b00) msg_state <= msg_state + 2'b01;
	if (msg_state == 2'b10)	msg_state <= 2'b00; //this is 01 or 00?

end
	
//Generate output messages for CPU
reg [31:0] msg_buf_in; 
wire [31:0] msg_buf_out;
reg msg_buf_wr;
wire msg_buf_rd, msg_buf_flush;
wire [7:0] msg_buf_size;
wire msg_buf_empty;

`define RED_BOX_MSG_ID "RBB"

always@(*) begin	//Write words to FIFO as state machine advances
	case(msg_state)
		2'b00: begin
			msg_buf_in = 32'b0;
			msg_buf_wr = 1'b0;
		end
		2'b01: begin
			msg_buf_in = `RED_BOX_MSG_ID;	//Message ID
			msg_buf_wr = 1'b1;
		end
		2'b10: begin
			msg_buf_in = {distance, angle, detected};
			msg_buf_wr = 1'b1;
		end
//		2'b10: begin
//			msg_buf_in = {5'b0, x_min, 5'b0, y_min};	//Top left coordinate
//			msg_buf_wr = 1'b1;
//		end
//		2'b11: begin
//			msg_buf_in = {5'b0, x_max, 5'b0, y_max}; //Bottom right coordinate
//			msg_buf_wr = 1'b1;
//		end
	endcase
end


//Output message FIFO
MSG_FIFO	MSG_FIFO_inst (
	.clock (clk),
	.data (msg_buf_in),
	.rdreq (msg_buf_rd),
	.sclr (~reset_n | msg_buf_flush),
	.wrreq (msg_buf_wr),
	.q (msg_buf_out),
	.usedw (msg_buf_size),
	.empty (msg_buf_empty)
	);


//Streaming registers to buffer video signal
STREAM_REG #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red,green,blue,sop,eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid),
	.data_in({red_out, green_out, blue_out, sop, eop})
);


/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define REG_STATUS    			0
`define READ_MSG    				1
`define READ_ID    				2
`define REG_BBCOL					3

//Status register bits
// 31:16 - unimplemented
// 15:8 - number of words in message buffer (read only)
// 7:5 - unused
// 4 - flush message buffer (write only - read as 0)
// 3:0 - unused


// Process write

reg  [7:0]   reg_status;
reg	[23:0]	bb_col;

always @ (posedge clk)
begin
	if (~reset_n)
	begin
		reg_status <= 8'b0;
		bb_col <= BB_COL_DEFAULT;
	end
	else begin
		if(s_chipselect & s_write) begin
		   if      (s_address == `REG_STATUS)	reg_status <= s_writedata[7:0];
		   if      (s_address == `REG_BBCOL)	bb_col <= s_writedata[23:0];
		end
	end
end


//Flush the message buffer if 1 is written to status register bit 4
assign msg_buf_flush = (s_chipselect & s_write & (s_address == `REG_STATUS) & s_writedata[4]);


// Process reads
reg read_d; //Store the read signal for correct updating of the message buffer

// Copy the requested word to the output port when there is a read.
always @ (posedge clk)
begin
   if (~reset_n) begin
	   s_readdata <= {32'b0};
		read_d <= 1'b0;
	end
	
	else if (s_chipselect & s_read) begin
		if   (s_address == `REG_STATUS) s_readdata <= {16'b0,msg_buf_size,reg_status};
		if   (s_address == `READ_MSG) s_readdata <= {msg_buf_out};
		if   (s_address == `READ_ID) s_readdata <= 32'h1234EEE2;
		if   (s_address == `REG_BBCOL) s_readdata <= {8'h0, bb_col};
	end
	
	read_d <= s_read;
end

//Fetch next word from message buffer after read from READ_MSG
assign msg_buf_rd = s_chipselect & s_read & ~read_d & ~msg_buf_empty & (s_address == `READ_MSG);
						


endmodule

