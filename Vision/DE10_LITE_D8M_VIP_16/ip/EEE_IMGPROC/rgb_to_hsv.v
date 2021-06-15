module  rgb_to_hsv(
	clk,
	reset_n,
	
	current_r,
	current_g,
	current_b,
	
	hsv_hue,		//0 to 360
	hsv_sat,		//0 to 255
	hsv_val		//0 to 255
	
);

// global clock & reset
input	clk;
input	reset_n;	

//input RGB red, green, blue
input [7:0] current_r;
input [7:0] current_g;
input [7:0] current_b;

//space conversion, output HSV
output reg [8:0] 	hsv_hue;
output reg [7:0]	hsv_sat;
output reg [7:0] 	hsv_val;


/////////////////////////////////////////////////////////////

reg [7:0] numer;
reg [13:0] num_times_60;//*60
reg [7:0] maximum, minimum, difference; //maximum, minimum, max-min
reg [7:0] hsv_sm;
reg [7:0] max_store;
reg [7:0] division;//division value
reg [2:0] rgb_state;
 
always @ (posedge clk or negedge reset_n)begin
	if (!reset_n) begin
		maximum <= 8'd0;
		minimum <= 8'd0;
		numer <= 8'd0;
		rgb_state <= 3'b010;
	end else if ((current_r >= current_b) && (current_r >= current_g) && (current_b >= current_g)) begin //R_B_G
		maximum <= current_r;
		minimum <= current_g;
		numer <= current_b - current_g;
		rgb_state <= 3'b000;

	end else if((current_r >= current_b) && (current_r >= current_g) && (current_b >= current_g)) begin //R_G_B
		maximum <= current_r;
		minimum <= current_b;
		numer <= current_g - current_b;
		rgb_state <= 3'b001;
		
	end else if((current_g >= current_b) && (current_g >= current_r) && (current_b >= current_r)) begin //G_B_R
		maximum <= current_g;
		minimum <= current_r;
		numer <= current_b - current_r;
		rgb_state <= 3'b011;
		
	end else if((current_g >= current_b) && (current_g >= current_r) && (current_r >= current_b)) begin //G_R_B
		maximum <= current_g;
		minimum <= current_b;
		numer <= current_r - current_b;
		rgb_state <= 3'b100;
		
	end else if((current_b >= current_r) && (current_b >= current_g) && (current_r >= current_g)) begin //B_R_G
		maximum <= current_b;
		minimum <= current_g;
		numer <= current_r - current_g;
		rgb_state <= 3'b101;
		
	end else if((current_b >= current_r) && (current_b >= current_g) && (current_g >= current_r)) begin //B_G_R
		maximum <= current_b;
		minimum <= current_r;
		numer <= current_g - current_r;
		rgb_state <= 3'b110;
	
	end
	
end

always @ (posedge clk or negedge reset_n) begin
	if (!reset_n) begin
		num_times_60 <= 14'd0;
		difference <= 8'd0;
		max_store <= 8'd0;
	end else begin
		num_times_60 <= {numer,6'b000000} - {numer,2'b00};
		difference <= maximum - minimum;
		max_store <= maximum;
	end
end

//always @ (posedge clk or negedge rst)
//begin
//	if (!rst)
//		begin
//		top_60 <= 14'd0;
//		rgb_se_n <= 3'b010;
//		max_min <= 8'd0;
//		max_n <= 8'd0;
//		end
//	else
//		begin
//		top_60 <= {top,6'b000000} - {top,2'b00};//60 = 2^6 - 2^2
//		rgb_se_n <= rgb_se;
//		max_min <= max - min;
//		max_n <= max;
//		end
//end

always @ (*) begin
	division = (difference > 8'd0) ? num_times_60 / difference : 8'd240;//Note that max = min  
end
// + - 120 240 360
always @ (posedge clk or negedge reset_n)
begin
	if (!reset_n)
	
		hsv_hue <= 9'd0;
	
	else 
	begin
	case (rgb_state)
	
	3'b000:
			//RED BLUE GREEN
			hsv_hue <= 9'd360 - division;//-
			
			
	3'b001:
			//R, GB
			hsv_hue <= division;//+

			
	3'b011:
			//G, BR
			hsv_hue <= 9'd120 + division;//+

			
	3'b100:
			//G, RB
			hsv_hue <= 9'd120 - division;//-

			
	3'b101:
			//B, RG
			hsv_hue <= 9'd240 + division;//+
			
			
	3'b111:
			//B, GR
			hsv_hue <= 9'd240 - division;//-
			
	default
			hsv_hue <= 9'd0;
	endcase
end
end

//  saturation=(maximum - mininimum)/maximum * 256
always @ (*)
begin
	hsv_sm = (max_store > 8'd0)? {difference[7:0],8'b00000000} / max_store : 8'd0;
end
always@(posedge clk or negedge reset_n)
begin
	if (!reset_n)
      hsv_sat <= 8'd0;
	else
		hsv_sat <= hsv_sm;
end
//  hsv_v = max
always@(posedge clk or negedge reset_n)
begin
  if (!reset_n)
  hsv_val <= 8'd0;
  else
  hsv_val <= max_store;
 end




endmodule