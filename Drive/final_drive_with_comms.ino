#include <Wire.h>
#include <INA219_WE.h>
//************************** SMPS Constants **************************//
INA219_WE ina219; // this is the instantiation of the library for the current sensor
// variables used for Serial Communication
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
char msg2[MSG_BUFFER_SIZE];
char recvFromControl[MSG_BUFFER_SIZE];
char tmpFromControl[MSG_BUFFER_SIZE];
boolean newData = false;
// variables received from CONTROL from COMMAND
char tmpMode[2];        // A for Auto, M for Manual, O for Obstacle Coordinates. 
int driveMode= 2;          // 0 for Automatic, 1 for Manual,2 is default. for DRIVE's usage
// automatic mode
int target_x= 1;           // target x-coordinate to travel to
int target_y= 800;           // target y-coordinate to travel to
long radius= 50;             // radius to sweep in automatic mode
int detected = 2;
// manual mode: can only be used after all obstacles have been detected
int cmd_direction;      // 0 for forwards, 1 for backwards
long cmd_dist;           // travel distance in terms of mm
int cmd_angle;          // -180 to 180, ACW is positive, CW is negative
int cmd_speed;          // rover speed

// variables to send to CONTROL, these values will actually change as the rover moves on the full Drive code
int rover_x = 0;
int rover_y = 0;
int steeringAngle = 0;
int prevX=0;
int prevY=0;
double prevD=0;


// obstacle coordinates received from CONTROL
int color0_x, color1_x, color2_x, color3_x, color4_x;
int color0_y, color1_y, color2_y, color3_y, color4_y;
int dx,dy;
int dx0, dx1, dx2, dx3, dx4;   
int dy0, dy1, dy2, dy3, dy4;   
float open_loop, closed_loop; // Duty Cycles
float vpd, vb, vref, iL, dutyref, current_mA; // Measurement Variables
unsigned int sensorValue0, sensorValue1, sensorValue2, sensorValue3; // ADC sample values declaration
float ev = 0, cv = 0, ei = 0, oc = 0; //internal signals
float Ts = 0.0008; //1.25 kHz control frequency. It's better to design the control period as integral multiple of switching period.
float kpv = 0.05024, kiv = 15.78, kdv = 0; // voltage pid.
float u0v, u1v, delta_uv, e0v, e1v, e2v; // u->output; e->error; 0->this time; 1->last time; 2->last last time
float kpi = 0.02512, kii = 39.4, kdi = 0; // current pid.
float u0i, u1i, delta_ui, e0i, e1i, e2i; // Internal values for the current controller
float uv_max = 4, uv_min = 0; //anti-windup limitation
float ui_max = 1, ui_min = 0; //anti-windup limitation
float current_limit = 1.0;
boolean Boost_mode = 0;
boolean CL_mode = 0;

unsigned int loopTrigger;
unsigned int com_count = 0; // a variables to count the interrupts. Used for program debugging.

//************************** Motor Constants **************************//
unsigned long previousMillis = 0; //initializing time counter
const long interval = 3000;

int DIRRstate = LOW;              //initializing direction states???forward???
int DIRLstate = HIGH;

int DIRL = 20;                    //defining left direction pin
int DIRR = 21;                    //defining right direction pin

int pwmr = 5;                     //pin to control right wheel speed using pwm
int pwml = 9;                     //pin to control left wheel speed using pwm


double target_a = 0;
int target_d = 0;

int mmangle = 0;
int mmdis = 0;

String mode;
int distancego = 100;
int anglego = 90;
int disvalue=0;
int angvalue=0;


int ball_x = 9999;
int ball_y = 9999;

int c = 0;         //coorection
int t = 0;         //turning
int f = 0;         //finish
int d = 0;         //detection
int v = 0;         //avoidance
int sa = 0;
int sd = 0;
int seq = 1;
int oper = 0;
//************************ Sensor Constant **************************//
#include "SPI.h"

#define PIN_SS        10
#define PIN_MISO      12
#define PIN_MOSI      11
#define PIN_SCK       13

#define PIN_MOUSECAM_RESET     8
#define PIN_MOUSECAM_CS        7

#define ADNS3080_PIXELS_X                 30
#define ADNS3080_PIXELS_Y                 30

#define ADNS3080_PRODUCT_ID            0x00
#define ADNS3080_REVISION_ID           0x01
#define ADNS3080_MOTION                0x02
#define ADNS3080_DELTA_X               0x03
#define ADNS3080_DELTA_Y               0x04
#define ADNS3080_SQUAL                 0x05
#define ADNS3080_PIXEL_SUM             0x06
#define ADNS3080_MAXIMUM_PIXEL         0x07
#define ADNS3080_CONFIGURATION_BITS    0x0a
#define ADNS3080_EXTENDED_CONFIG       0x0b
#define ADNS3080_DATA_OUT_LOWER        0x0c
#define ADNS3080_DATA_OUT_UPPER        0x0d
#define ADNS3080_SHUTTER_LOWER         0x0e
#define ADNS3080_SHUTTER_UPPER         0x0f
#define ADNS3080_FRAME_PERIOD_LOWER    0x10
#define ADNS3080_FRAME_PERIOD_UPPER    0x11
#define ADNS3080_MOTION_CLEAR          0x12
#define ADNS3080_FRAME_CAPTURE         0x13
#define ADNS3080_SROM_ENABLE           0x14
#define ADNS3080_FRAME_PERIOD_MAX_BOUND_LOWER      0x19
#define ADNS3080_FRAME_PERIOD_MAX_BOUND_UPPER      0x1a
#define ADNS3080_FRAME_PERIOD_MIN_BOUND_LOWER      0x1b
#define ADNS3080_FRAME_PERIOD_MIN_BOUND_UPPER      0x1c
#define ADNS3080_SHUTTER_MAX_BOUND_LOWER           0x1e
#define ADNS3080_SHUTTER_MAX_BOUND_UPPER           0x1e
#define ADNS3080_SROM_ID               0x1f
#define ADNS3080_OBSERVATION           0x3d
#define ADNS3080_INVERSE_PRODUCT_ID    0x3f
#define ADNS3080_PIXEL_BURST           0x40
#define ADNS3080_MOTION_BURST          0x50
#define ADNS3080_SROM_LOAD             0x60

#define ADNS3080_PRODUCT_ID_VAL        0x17
int star_x = 0;
int star_y = 0;

int total_x = 0;
int total_y = 0;
int total_a = 0;

long total_a1 = 0;
long total_x1 = 0;
long total_y1 = 0;
long head_x = 0;
long head_y = 0;

int x = 0;
int y = 0;

int a = 0;
int b = 0;

int distance_x = 0;
int distance_y = 0;

double Dir_steer = 0;
double Dir_steerd = 0;

volatile byte movementflag = 0;
volatile int xydat[2];
const float Pi = 3.14159;

//*****************************sensor function***********************//
int convTwosComp(int b) {
  //Convert from 2's complement
  if (b & 0x80) {
    b = -1 * ((b ^ 0xff) + 1);
  }
  return b;
}

int tdistance = 0;

void mousecam_reset()
{
  digitalWrite(PIN_MOUSECAM_RESET, HIGH);
  delay(1); // reset pulse >10us
  digitalWrite(PIN_MOUSECAM_RESET, LOW);
  delay(35); // 35ms from reset to functional
}

int mousecam_init()
{
  pinMode(PIN_MOUSECAM_RESET, OUTPUT);
  pinMode(PIN_MOUSECAM_CS, OUTPUT);

  digitalWrite(PIN_MOUSECAM_CS, HIGH);

  mousecam_reset();

  int pid = mousecam_read_reg(ADNS3080_PRODUCT_ID);
  if (pid != ADNS3080_PRODUCT_ID_VAL)
    return -1;

  // turn on sensitive mode
  mousecam_write_reg(ADNS3080_CONFIGURATION_BITS, 0x19);

  return 0;
}

void mousecam_write_reg(int reg, int val)
{
  digitalWrite(PIN_MOUSECAM_CS, LOW);
  SPI.transfer(reg | 0x80);
  SPI.transfer(val);
  digitalWrite(PIN_MOUSECAM_CS, HIGH);
  delayMicroseconds(50);
}

int mousecam_read_reg(int reg)
{
  digitalWrite(PIN_MOUSECAM_CS, LOW);
  SPI.transfer(reg);
  delayMicroseconds(75);
  int ret = SPI.transfer(0xff);
  digitalWrite(PIN_MOUSECAM_CS, HIGH);
  delayMicroseconds(1);
  return ret;
}

struct MD
{
  byte motion;
  char dx, dy;
  byte squal;
  word shutter;
  byte max_pix;
};


void mousecam_read_motion(struct MD *p)
{
  digitalWrite(PIN_MOUSECAM_CS, LOW);
  SPI.transfer(ADNS3080_MOTION_BURST);
  delayMicroseconds(75);
  p->motion =  SPI.transfer(0xff);
  p->dx =  SPI.transfer(0xff);
  p->dy =  SPI.transfer(0xff);
  p->squal =  SPI.transfer(0xff);
  p->shutter =  SPI.transfer(0xff) << 8;
  p->shutter |=  SPI.transfer(0xff);
  p->max_pix =  SPI.transfer(0xff);
  digitalWrite(PIN_MOUSECAM_CS, HIGH);
  delayMicroseconds(5);
}

// pdata must point to an array of size ADNS3080_PIXELS_X x ADNS3080_PIXELS_Y
// you must call mousecam_reset() after this if you want to go back to normal operation
int mousecam_frame_capture(byte *pdata)
{
  mousecam_write_reg(ADNS3080_FRAME_CAPTURE, 0x83);

  digitalWrite(PIN_MOUSECAM_CS, LOW);

  SPI.transfer(ADNS3080_PIXEL_BURST);
  delayMicroseconds(50);

  long pix;
  byte started = 0;
  long count;
  int timeout = 0;
  long ret = 0;
  for (count = 0; count < ADNS3080_PIXELS_X * ADNS3080_PIXELS_Y; )
  {
    pix = SPI.transfer(0xff);
    delayMicroseconds(10);
    if (started == 0)
    {
      if (pix & 0x40)
        started = 1;
      else
      {
        timeout++;
        if (timeout == 100)
        {
          ret = -1;
          break;
        }
      }
    }
    if (started == 1)
    {
      pdata[count++] = (pix & 0x3f) << 2; // scale to normal grayscale byte range
    }
  }

  digitalWrite(PIN_MOUSECAM_CS, HIGH);
  delayMicroseconds(14);

  return ret;
}


char asciiart(int k)
{
  static char foo[] = "WX86*3I>!;~:,`. ";
  return foo[k >> 4];
}

byte frame[ADNS3080_PIXELS_X * ADNS3080_PIXELS_Y];

double steering_direction(int x)
{
  double theta = x * 0.405;
  if (theta < -180) {
    theta = theta + 360;
  }
  else if (theta > 180) {
    theta = theta - 360;
  }
  return theta;
}

double returndeg(int xtarget, int ytarget, int xcurrent, int ycurrent)
{
  int xdiff = xtarget - xcurrent;
  int ydiff = ytarget - ycurrent;
  double diffratio = xdiff / ydiff;
  double degdiff = atan(diffratio) * 180 / Pi;           //atan only able to return angles at first and fourth quardrant
  if (ydiff < 0) {
    if (degdiff > 0) {                                   //works for all quardrant (need test) 
      degdiff = 180 - degdiff;
    }
    if (degdiff < 0) {
      degdiff = -180 - degdiff;
    }
  }
  return degdiff;
}

int returndis(int xtarget, int ytarget, int xcurrent, int ycurrent) {
  long xdiff = long(xtarget - xcurrent);
  long ydiff = long(ytarget - ycurrent);
  long xsqr = xdiff * xdiff;
  long ysqr = ydiff * ydiff;
  long diff2 = xsqr + ysqr;                                                //use long() to convert int to long able to return very larger distance
  int disdiff = round(sqrt(diff2));
  return disdiff;
}
void setup() {
  //************************** Motor Pins Defining **************************//
  pinMode(DIRR, OUTPUT);
  pinMode(DIRL, OUTPUT);
  pinMode(pwmr, OUTPUT);
  pinMode(pwml, OUTPUT);
  digitalWrite(DIRR, LOW);
  digitalWrite(DIRL, HIGH);
  digitalWrite(pwmr, LOW);       //setting right motor speed at maximum
  digitalWrite(pwml, LOW);       //setting left motor speed at maximum
  //*******************************************************************//
  //Basic pin setups

  noInterrupts(); //disable all interrupts
  pinMode(13, OUTPUT);  //Pin13 is used to time the loops of the controller
  pinMode(3, INPUT_PULLUP); //Pin3 is the input from the Buck/Boost switch
  pinMode(2, INPUT_PULLUP); // Pin 2 is the input from the CL/OL switch
  analogReference(EXTERNAL); // We are using an external analogue reference for the ADC

  // TimerA0 initialization for control-loop interrupt.

  TCA0.SINGLE.PER = 999; //
  TCA0.SINGLE.CMP1 = 999; //
  TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV16_gc | TCA_SINGLE_ENABLE_bm; //16 prescaler, 1M.
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP1_bm;

  // TimerB0 initialization for PWM output

  pinMode(6, OUTPUT);
  TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm; //62.5kHz
  analogWrite(6, 120);

  interrupts();  //enable interrupts.
  Wire.begin(); // We need this for the i2c comms for the current sensor
  ina219.init(); // this initiates the current sensor
  Wire.setClock(700000); // set the comms speed for i2c
  //********************Sensor Pins Defining*******************//
  pinMode(PIN_SS, OUTPUT);
  pinMode(PIN_MISO, INPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_SCK, OUTPUT);

  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);

  Serial.begin(38400);
  Serial1.begin(9600);
  if (mousecam_init() == -1)
  {
    Serial.println("Mouse cam failed to init");
    while (1);
  }
  enable();
}
//--------------motor movement-----------------------
void enable() {
  digitalWrite(pwmr, HIGH);
  digitalWrite(pwml, HIGH);
}
void disable() {
  digitalWrite(pwmr, LOW);
  digitalWrite(pwml, LOW);
}
void forward() {
  digitalWrite(DIRR, HIGH);
  digitalWrite(DIRL, LOW);
}
void backward() {
  digitalWrite(DIRR, LOW);
  digitalWrite(DIRL, HIGH);
}
void clockwise() {
  digitalWrite(DIRR, LOW);
  digitalWrite(DIRL, LOW);
}
void anticlockwise() {
  digitalWrite(DIRR, HIGH);
  digitalWrite(DIRL, HIGH);
}
void speedcontrol(int value){    //0 to 5  from lowest to maximum speed  5 speeds
vref=value*4.096/5;
}


void movebydistance(int distance) {
  if (sd == 0) {                                      //store the distance we want to move to distancego at the start of function only once
    distancego = distance;
    sd = 1;
  }
  if (sd == 1) {
    int mmdis = 10 * distance_y / 157;
    distancego = distancego - mmdis;    
    if (abs(distancego) >2){
      if (distance >0){
        forward();
      }else{
        backward();
      }
    
      if (abs(distancego)<20){
        speedcontrol(1);
      }else{
        speedcontrol(cmd_speed);
    }
  }else{
    sd = 0;
    seq = seq +1;
  }
 }
}
  

void movebyangle (int value) {                 //store the angle at the begining of the function only once
  if (sa == 0) {
    mmangle = Dir_steerd;
    anglego = value + mmangle;
    if (anglego < -180) {                       //make sure after addition it still in the range of -180 to 180
      anglego = anglego + 360;
    }
    else if (anglego > 180) {
      anglego = anglego - 360;
    }
    sa = 1;
  }
  else if (sa == 1) {
    long errorang=round(abs(value/8));
    Serial.println(errorang);
    if (value > 0 && abs(anglego - Dir_steerd) > errorang) {          //turning direction depend on the sign of the angle
      anticlockwise();
    }
    else if (value < 0 && abs(anglego - Dir_steerd) > errorang) {
      clockwise();
    }
    else {                        //seq increase at the end of function
      sa = 0;
      seq = seq + 1;
      errorang=0;
    }
  }
}


void obsdetect() {
  switch (seq) {
    case 1:
      movebydistance(radius);
      break;
    case 2:
      movebyangle(90);
      break;
    case 3:
      movebydistance(radius);
      break;
    case 4:
      movebyangle(90);
      break;
    case 5:
      movebydistance(2 * radius);
      break;
    case 6:
      movebyangle(90);
      break;
    case 7:
      movebydistance(2 * radius);
      break;
    case 8:
      movebyangle(90);
      break;
    case 9:
      movebydistance(2 * radius);
      break;
    case 10:
      movebyangle(90);
      break;
    case 11:
      movebydistance(radius);
      break;
    case 12:
      movebyangle(-90);
      break;
    case 13:
      movebydistance(-radius);
      break;
    case 14:
      d = 1;
      seq=1;   //reset variable seq to prepare for the obsavoid
      disable();
      break;
  }
}

void obsavoid() {                           // didn't tested but similar strucutre with obsdetect should not have too mang issues
  Serial.println("avoiding");
  if (dx0 > 0) {                           // when dx>100 turn left to avoid it
    if (oper == 0) {
      int mmdis = 10 * distance_y / 157;
      dy0 = dy0 - mmdis;
      if (dy0 > 105) {
        forward();
      }
      else if (dy0 < 95) {
        backward();
      }
      else {
        oper = 1;                            //move close enough
      }
    }
    if (oper == 1) {
      switch (seq) {
        case 1:
          movebyangle(90);
          break;
        case 2:
          movebydistance(dx0 + 220);
          break;
        case 3:
          movebyangle(-90);
          break;
        case 4:
          movebydistance(440);
          break;
        case 5:
          movebyangle(-90);
          break;
        case 6:
          movebydistance(dx0+ 220);
          break;
        case 7:
          movebyangle(90);
          break;
        case 8:
          v = 1; 
          
          disable();//this function can only run once
          break;
          disable();
      }
    }
  }
  if (dx0 < 0) {
    backward();
    Serial.println("smaller than 0");
    if (oper == 0) {
      int mmdis = 10 * distance_y / 157;
      dy0 = dy0 - mmdis;
      Serial.println("initialized dy0="+String(dy0));
      if (dy0 < 95) {
        backward();
      }
      else {
        oper = 1;
      }
    }
    if (oper == 1) {
      Serial.println("avoiding start");
      switch (seq) {
        case 1:
          movebyangle(-90);
          break;
        case 2:
          movebydistance(dx0 + 220);
          break;
        case 3:
          movebyangle(90);
          break;
        case 4:
          movebydistance(440);
          break;
        case 5:
          movebyangle(90);
          break;
        case 6:
          movebydistance(dx0 + 220);
          break;
        case 7:
          movebyangle(-90);
          break;
        case 8:
          v = 1;
          break;
      }
    }
    else {
      v = 1;
      disable();
    }
  }
}
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval){//interval 1 second
    previousMillis=currentMillis;
    getControlData();
    sendToControl();
    //Serial.println("activated");
  }

  if (loopTrigger) { // This loop is triggered, it wont run unless there is an interrupt

    digitalWrite(13, HIGH);   // set pin 13. Pin13 shows the time consumed by each control cycle. It's used for debugging.

    // Sample all of the measurements and check which control mode we are in
    sampling();
    CL_mode = digitalRead(3); // input from the OL_CL switch
    Boost_mode = digitalRead(2); // input from the Buck_Boost switch


    if (Boost_mode) {
      if (CL_mode) { //Closed Loop Boost
        pwm_modulate(1); // This disables the Boost as we are not using this mode
      } else { // Open Loop Boost
        pwm_modulate(1); // This disables the Boost as we are not using this mode
      }
    } else {
      if (CL_mode) { // Closed Loop Buck
        // The closed loop path has a voltage controller cascaded with a current controller. The voltage controller
        // creates a current demand based upon the voltage error. This demand is saturated to give current limiting.
        // The current loop then gives a duty cycle demand based upon the error between demanded current and measured
        // current
        current_limit = 3; // Buck has a higher current limit
        ev = vref - vb;  //voltage error at this time
        cv = pidv(ev); //voltage pid
        cv = saturation(cv, current_limit, 0); //current demand saturation
        ei = cv - iL; //current error
        closed_loop = pidi(ei); //current pid
        closed_loop = saturation(closed_loop, 0.99, 0.01); //duty_cycle saturation
        pwm_modulate(closed_loop); //pwm modulation
      } else { // Open Loop Buck
        current_limit = 3; // Buck has a higher current limit
        oc = iL - current_limit; // Calculate the difference between current measurement and current limit
        if ( oc > 0) {
          open_loop = open_loop - 0.001; // We are above the current limit so less duty cycle
        } else {
          open_loop = open_loop + 0.001; // We are below the current limit so more duty cycle
        }
        open_loop = saturation(open_loop, dutyref, 0.02); // saturate the duty cycle at the reference or a min of 0.01
        pwm_modulate(open_loop); // and send it out
      }
    }
    // closed loop control path

    digitalWrite(13, LOW);   // reset pin13.
    loopTrigger = 0;
  }
  //************************** Motor test **************************//
  //auto mode-coordinates
    //auto mode-coordinates
  if (driveMode==0) {                                                     //auto mode-coordinates
    if (d == 0) {                                //d=0 do obstacle detection. only once since d will equals 1 at the end of obsdetect()
      obsdetect();
    }
    if (d == 1) {                                //after finish detection start polar coordinate control
      if (c == 0 && t == 0) {                    //c=0 means need to do self correction
        star_x = total_x;
        star_y = total_y;
        target_a = returndeg(target_x, target_y, star_x, star_y);
        target_d = returndis(target_x, target_y, star_x, star_y);
        c = 1;                                                                    //one means correction done
      }
      if (c == 1 && t == 0) {                     // c=1 means correction finished and t=0 means turning is not finished
        if ((target_a - Dir_steerd) > 5) {                                        //choose to rotate in CCW
          anticlockwise();
        }
        else if ((target_a - Dir_steerd) < -5) {                                  //choose to rotate in CW
          clockwise();
        }
        else {                                                                    //turning has done
          forward();
          t = 1;                                 //after turning switch to forward mode t=1 means turning done
        }
      }
      if (t == 1 && c == 1) {                    //turning done and self correction done
        int movepolar = returndis(total_x, total_y, star_x, star_y); //distance between current position and the position of  last correction
        if (((target_d - movepolar) < 0) && v == 1) {                              //move to the right position
          f = 1;                                 //f =1 means it reach the right position and motors turn off
        }
        if ( (dx + dy) > 0) {                    //if there is a obstacle          
          obsavoid();
        }
        else if ((dx + dy) == 0) {
          v = 1;                                 //if there is no obstacle v=1 means either there is no obstacle or it has been avoided
        }
        if ((200 - movepolar) < 0) {                                              //run over 200mm needs to do correction
          t = 0;
          c = 0;
        }
      }
      if (f == 1) {
        disable();
        c=0;
        t=0;
      }
    }
  }
  //manual mode - move by distance&angle
  if (driveMode == 1) {
    delay(500);
    if (cmd_angle==0){
      if (cmd_direction == 0) {
        movebydistance(cmd_dist);
       if (sd == 0) {                                      //store the distance we want to move to distancego at the start of function only once
        distancego = cmd_dist;
        Serial.println("distancego is"+String(distancego));
        sd = 1;   
      }
        if (sd == 1) {
          int mmdis = 10 * distance_y / 157;
          distancego = distancego - mmdis;
          if (distancego > 2) {
            forward();
          }
          else if (distancego < -2) {
            backward();
          }
          else {
            sd = 0;
            seq = seq + 1;
            disable();
          }
        }
      }
      if (cmd_direction == 1) {
        
        if (sd == 0) {                                      //store the distance we want to move to distancego at the start of function only once
          distancego = -cmd_dist;
          Serial.println("distancego is"+String(distancego));
          sd = 1;   
      }
        if (sd == 1) {
          int mmdis = 10 * distance_y / 157;
          distancego = distancego - mmdis;
          if (distancego > 2) {
            forward();
          }
          else if (distancego < -2) {
            backward();
          }
          else {
            sd = 0;
            seq = seq + 1;
            disable();
          }
        };
      }
    }
    else{
      movebyangle(cmd_angle);                         //try to avoid moving over 180degree sign change could have unexpected result
    }

    //manual mode -stop when approaching an obstacle
    float taildiff = returndis(ball_x, ball_y, total_x, total_y);
    float headdiff = returndis(ball_x, ball_y, head_x, head_y);
    if (taildiff < 220 || headdiff < 220) {
      disable();
      Serial.println("warning");
      Serial.println('\n');
    }
  }
  
  //*******************************************************************//

#if 0

  /*
      if(movementflag){

      tdistance = tdistance + convTwosComp(xydat[0]);
      Serial.println("Distance = " + String(tdistance));
      movementflag=0;
      delay(3);
      }

  */
  // if enabled this section grabs frames and outputs them as ascii art

  if (mousecam_frame_capture(frame) == 0)
  {
    long i, j, k;
    for (i = 0, k = 0; i < ADNS3080_PIXELS_Y; i++)
    {
      for (j = 0; j < ADNS3080_PIXELS_X; j++, k++)
      {
        Serial.print(asciiart(frame[k]));
        Serial.print(' ');
      }
      Serial.println();
    }
  }
  Serial.println();
  delay(100);

#else

  // if enabled this section produces a bar graph of the surface quality that can be used to focus the camera
  // also drawn is the average pixel value 0-63 and the shutter speed and the motion dx,dy.
  int val = mousecam_read_reg(ADNS3080_PIXEL_SUM);
  MD md;
  mousecam_read_motion(&md);
  for (int i = 0; i < md.squal / 4; i++)
    Serial.print('*');
  Serial.print(' ');
  Serial.print((val * 100) / 351);
  Serial.print(' ');
  Serial.print(md.shutter); Serial.print(" (");
  Serial.print((int)md.dx); Serial.print(',');
  Serial.print((int)md.dy); Serial.println(')');
  delay(100);
  distance_x = md.dx; //convTwosComp(md.dx);
  distance_y = md.dy; //convTwosComp(md.dy);

  total_a1 = total_a1 + distance_x;
  total_a  = 10 * total_a1 / 157;

  Dir_steerd = steering_direction(total_a);
  Dir_steer = Dir_steerd * Pi / 180;

  total_x1 = total_x1 + round(distance_y * sin(Dir_steer));
  total_y1 = total_y1 + round(distance_y * cos(Dir_steer));

  total_x = 10 * total_x1 / 157; //Conversion from counts per inch to mm (400 counts per inch)
  total_y = 10 * total_y1 / 157; //Conversion from counts per inch to mm (400 counts per inch)

  head_x = total_x + round (220 * sin(Dir_steer));
  head_y = total_y + round (220 * cos(Dir_steer));

  Serial.print('\n');
  Serial.print("rover position:"); Serial.print(" (");
  Serial.print((int)total_x); Serial.print(',');
  Serial.print((int)total_y); Serial.println(')');
  Serial.println("steering direction = " + String(Dir_steerd));
  delay(100);

#endif
}

//************************************motor function************************************//
// Timer A CMP1 interrupt. Every 800us the program enters this interrupt.
// This, clears the incoming interrupt flag and triggers the main loop.

ISR(TCA0_CMP1_vect) {
  TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_CMP1_bm; //clear interrupt flag
  loopTrigger = 1;
}


// This subroutine processes all of the analogue samples, creating the required values for the main loop

void sampling() {

  // Make the initial sampling operations for the circuit measurements

  sensorValue0 = analogRead(A0); //sample Vb
  sensorValue2 = analogRead(A2); //sample Vref
  sensorValue3 = analogRead(A3); //sample Vpd
  current_mA = ina219.getCurrent_mA(); // sample the inductor current (via the sensor chip)

  // Process the values so they are a bit more usable/readable
  // The analogRead process gives a value between 0 and 1023
  // representing a voltage between 0 and the analogue reference which is 4.096V

  vb = sensorValue0 * (4.096 / 1023.0); // Convert the Vb sensor reading to volts
  //vref = sensorValue2 * (4.096 / 1023.0); // Convert the Vref sensor reading to volts
  vpd = sensorValue3 * (4.096 / 1023.0); // Convert the Vpd sensor reading to volts

  // The inductor current is in mA from the sensor so we need to convert to amps.
  // We want to treat it as an input current in the Boost, so its also inverted
  // For open loop control the duty cycle reference is calculated from the sensor
  // differently from the Vref, this time scaled between zero and 1.
  // The boost duty cycle needs to be saturated with a 0.33 minimum to prevent high output voltages

  if (Boost_mode == 1) {
    iL = -current_mA / 1000.0;
    dutyref = saturation(sensorValue2 * (1.0 / 1023.0), 0.99, 0.33);
  } else {
    iL = current_mA / 1000.0;
    dutyref = sensorValue2 * (1.0 / 1023.0);
  }

}

float saturation( float sat_input, float uplim, float lowlim) { // Saturatio function
  if (sat_input > uplim) sat_input = uplim;
  else if (sat_input < lowlim ) sat_input = lowlim;
  else;
  return sat_input;
}

void pwm_modulate(float pwm_input) { // PWM function
  analogWrite(6, (int)(255 - pwm_input * 255));
}



// This is a PID controller for the voltage

float pidv( float pid_input) {
  float e_integration;
  e0v = pid_input;
  e_integration = e0v;

  //anti-windup, if last-time pid output reaches the limitation, this time there won't be any intergrations.
  if (u1v >= uv_max) {
    e_integration = 0;
  } else if (u1v <= uv_min) {
    e_integration = 0;
  }

  delta_uv = kpv * (e0v - e1v) + kiv * Ts * e_integration + kdv / Ts * (e0v - 2 * e1v + e2v); //incremental PID programming avoids integrations.there is another PID program called positional PID.
  u0v = u1v + delta_uv;  //this time's control output

  //output limitation
  saturation(u0v, uv_max, uv_min);

  u1v = u0v; //update last time's control output
  e2v = e1v; //update last last time's error
  e1v = e0v; // update last time's error
  return u0v;
}

// This is a PID controller for the current

float pidi(float pid_input) {
  float e_integration;
  e0i = pid_input;
  e_integration = e0i;

  //anti-windup
  if (u1i >= ui_max) {
    e_integration = 0;
  } else if (u1i <= ui_min) {
    e_integration = 0;
  }

  delta_ui = kpi * (e0i - e1i) + kii * Ts * e_integration + kdi / Ts * (e0i - 2 * e1i + e2i); //incremental PID programming avoids integrations.
  u0i = u1i + delta_ui;  //this time's control output

  //output limitation
  saturation(u0i, ui_max, ui_min);

  u1i = u0i; //update last time's control output
  e2i = e1i; //update last last time's error
  e1i = e0i; // update last time's error
  return u0i;
}
//*******************************************************//
//--------ctrl functions----------
// Send rover data to CONTROL every second
void sendToControl() {
  if((total_x!=prevX)||(total_y!=prevY)||(Dir_steerd!=prevD)){//write only if the situation changes 
    prevX=-total_x;
    prevY=total_y;
    prevD=Dir_steerd;
    snprintf (msg, MSG_BUFFER_SIZE, "<%i,%i,%i>", -total_x, total_y, int(Dir_steerd));
    Serial1.write(msg);//send the current position to esp32
    Serial.println("To control: current position of rover:"+String(msg)); //for debugging
  }
}

void getControlData() {
  recvFromSerial1();
  if (newData == true) {
    detected = 0;
    strcpy(tmpFromControl, recvFromControl);
    parseControlData();
    printControlData();   // for debugging
    newData = false;
    enable();
  }
}

void recvFromSerial1() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        recvFromControl[ndx] = rc;
        ndx++;
        if (ndx >= MSG_BUFFER_SIZE) {
          ndx = MSG_BUFFER_SIZE - 1;
        }
      }
      else {
        recvFromControl[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void parseControlData() {
  char * strtokIndx;
  int tmpColor;
  strtokIndx = strtok(recvFromControl, ",");
  strcpy(tmpMode, strtokIndx);
  // automatic mode
  if (strcmp(tmpMode, "A") == 0) {
    strtokIndx = strtok(NULL, ",");
    target_x = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_y = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    radius = atoi(strtokIndx);
    driveMode = 0;
  }
  // manual mode
  if (strcmp(tmpMode, "M") == 0) {
    strtokIndx = strtok(NULL, ",");
    cmd_direction = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_dist = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_angle = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_speed = atoi(strtokIndx);
    driveMode = 1;
    Serial.println("drivemode = "+String(driveMode));
  }


  // receiving obstacle coordinates from CONTROL
  if (strcmp(tmpMode, "O") == 0) {
    detected = 1;
    strtokIndx = strtok(NULL, ",");
    tmpColor = atoi(strtokIndx);
    switch (tmpColor) {
      case 0:
        strtokIndx = strtok(NULL, ",");
        color0_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color0_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx0 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy0 = atoi(strtokIndx);
        dx = dx0;
        dy = dy0;
        break;
      case 1:
        strtokIndx = strtok(NULL, ",");
        color1_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color1_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx1 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy1 = atoi(strtokIndx);
        dx = dx1;
        dy = dy1;        
        break;
      case 2:
        strtokIndx = strtok(NULL, ",");
        color2_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color2_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx2 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy2 = atoi(strtokIndx);
        dx = dx2;
        dy = dy2; 
        break;
      case 3:
        strtokIndx = strtok(NULL, ",");
        color3_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color3_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx3 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy3 = atoi(strtokIndx);
        dx = dx3;
        dy = dy3; 
        break;
      case 4:
        strtokIndx = strtok(NULL, ",");
        color4_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color4_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx4 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy4 = atoi(strtokIndx);
        dx = dx4;
        dy = dy4; 
        break;
    }
  }
}

// for debugging, print data received from CONTROL to serial monitor
void printControlData(){
  if (strcmp(tmpMode, "A") == 0) {
    Serial.println("Automatic mode selected.");
    Serial.print("driveMode: ");
    Serial.println(driveMode);
    Serial.print("target_x: ");
    Serial.println(target_x);
    Serial.print("target_y: ");
    Serial.println(target_y);
    Serial.print("radius: ");
    Serial.println(radius);
  }
  if (strcmp(tmpMode, "M") == 0) {
    Serial.println("Manual mode selected.");
    Serial.print("driveMode: ");
    Serial.println(driveMode);
    Serial.print("direction (0 for forwards, 1 for backwards): ");
    Serial.println(cmd_direction);
    Serial.print("distance to travel: ");
    Serial.println(cmd_dist);
    Serial.print("desired rover angle: ");
    Serial.println(cmd_angle);
    Serial.print("desired rover speed: ");
    Serial.println(cmd_speed);
  }
  if (strcmp(tmpMode, "O") == 0) {
    Serial.println("Obstacle coordinates received. ");
    Serial.print("color0 x: ");
    Serial.print(color0_x);
    Serial.print(", color0 y: ");
    Serial.print(color0_y);
    Serial.print(", dx0: ");
    Serial.print(dx0);
    Serial.print(", dy0: ");
    Serial.println(dy0);
    Serial.print("color1 x: ");
    Serial.print(color1_x);
    Serial.print(", color1 y: ");
    Serial.print(color1_y);
    Serial.print(", dx1: ");
    Serial.print(dx1);
    Serial.print(", dy1: ");
    Serial.println(dy1);
    Serial.print("color2 x: ");
    Serial.print(color2_x);
    Serial.print(", color2 y: ");
    Serial.print(color2_y);
    Serial.print(", dx2: ");
    Serial.print(dx2);
    Serial.print(", dy2: ");
    Serial.println(dy2);
    Serial.print("color3 x: ");
    Serial.print(color3_x);
    Serial.print(", color3 y: ");
    Serial.print(color3_y);
    Serial.print(", dx3: ");
    Serial.print(dx3);
    Serial.print(", dy3: ");
    Serial.println(dy3);
    Serial.print("color4 x: ");
    Serial.print(color4_x);
    Serial.print(", color4 y: ");
    Serial.print(color4_y);
    Serial.print(", dx4: ");
    Serial.print(dx4);
    Serial.print(", dy4: ");
    Serial.println(dy4);
  }
}

//*******************************************************//
