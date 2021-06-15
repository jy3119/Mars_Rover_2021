#include <Wire.h>

// variables received from VISION
long BASE_ADDRESS = 0x80000;                                            // check platform designer in Quartus for BASE_ADDRESS of I2C_MEM
int8_t byte0, byte1, byte2, byte3;                                      // for storing data read from VISION
int detected0, detected1, detected2, detected3, detected4;              // 1 only if obstacle is detected, 0 other wise
int diag_dist0, diag_dist1, diag_dist2, diag_dist3, diag_dist4;         // diagonal distance of obstacle to rover, received in cm
int hori_dist0, hori_dist1, hori_dist2, hori_dist3, hori_dist4;         // horizontal distance of obstacle to rover, received in mm
double angle0, angle1, angle2, angle3, angle4;                          // angle of osbtacle to rover. on left of rover = +ve,  on right of rover = -ve.  range: -90 to 90 degrees
double angle0deg, angle1deg, angle2deg, angle3deg, angle4deg;

void setup() {
  Wire.begin();             // start I2C connection with vision's FPGA
  Serial.begin(115200);     // Serial Monitor for ESP32
}

void loop() {
  // receiving data from FPGA via I2C protocol
  getVisionData();
}

/*================================= VISION FUNCTIONS =================================*/
// set the memory address on VISION's FPGA to read from for I2C communication
void setAddress(long addr)  {
  Wire.beginTransmission(0x55);
  Wire.write(addr >> 24);
  Wire.write((addr >> 16) & 255);
  Wire.write((addr >> 8) & 255);
  Wire.write(addr & 255);
  Wire.endTransmission(false);
}

// receive obstacle data from VISION via I2C
void getVisionData(){
  getVisionData0();
  getVisionData1();
  getVisionData2();
  getVisionData3();
  getVisionData4();
  // printVisionData();      // for debugging
}
void getVisionData0() {
  setAddress(BASE_ADDRESS);
  Wire.requestFrom(0x55, 4);    // request 1 word (4 bytes) from FPGA
  if (Wire.available() > 0) {
    // read incoming bytes from I2C
    byte0 = (Wire.read());
    byte1 = (Wire.read());
    byte2 = (Wire.read());
    byte3 = (Wire.read());
    // store values read from VISION into variables
    detected0 = byte0;
    diag_dist0 = byte1*10;
    hori_dist0 = ((byte3 << 8) | byte2);      // concat 2 bytes into int
    double ratio = (double)hori_dist0 / (double)diag_dist0;
    angle0 = asin(ratio); 
    angle0deg = (angle0 * 4068) / 71.0;
  }
}
void getVisionData1() {
  setAddress(BASE_ADDRESS+4);
  Wire.requestFrom(0x55, 4);    
  if (Wire.available() > 0) {
    byte0 = (Wire.read());
    byte1 = (Wire.read());
    byte2 = (Wire.read());
    byte3 = (Wire.read());
    detected1 = byte0;
    diag_dist1 = byte1*10;
    hori_dist1 = ((byte3 << 8) | byte2);
    double ratio = (double)hori_dist1 / (double)diag_dist1;
    angle1 = asin(ratio); 
    angle1deg = (angle1 * 4068) / 71.0;
  }
}
void getVisionData2() {
  setAddress(BASE_ADDRESS+8);
  Wire.requestFrom(0x55, 4);    
  if (Wire.available() > 0) {
    byte0 = (Wire.read());
    byte1 = (Wire.read());
    byte2 = (Wire.read());
    byte3 = (Wire.read());
    detected2 = byte0;
    diag_dist2 = byte1*10;
    hori_dist2 = ((byte3 << 8) | byte2);
    double ratio = (double)hori_dist2 / (double)diag_dist2;
    angle2 = asin(ratio);
    angle2deg = (angle2 * 4068) / 71.0; 
  }
}
void getVisionData3() {
  setAddress(BASE_ADDRESS+12);
  Wire.requestFrom(0x55, 4);    
  if (Wire.available() > 0) {
    byte0 = (Wire.read());
    byte1 = (Wire.read());
    byte2 = (Wire.read());
    byte3 = (Wire.read());
    detected3 = byte0;
    diag_dist3 = byte1*10;
    hori_dist3 = ((byte3 << 8) | byte2);
    double ratio = (double)hori_dist3 / (double)diag_dist3;
    angle3 = asin(ratio);
    angle3deg = (angle3 * 4068) / 71.0; 
  }
}
void getVisionData4() {
  setAddress(BASE_ADDRESS+16);
  Wire.requestFrom(0x55, 4);    
  if (Wire.available() > 0) {
    byte0 = (Wire.read());
    byte1 = (Wire.read());
    byte2 = (Wire.read());
    byte3 = (Wire.read());
    detected4 = byte0;
    hori_dist4 = ((byte3 << 8) | byte2);
    diag_dist4 = byte1*10;
    double ratio = (double)hori_dist4 / (double)diag_dist4;
    angle4 = asin(ratio); 
    angle4deg = (angle4 * 4068) / 71.0; 
  }
}

// for debugging, print out data received from VISION to serial monitor
void printVisionData() {
  Serial.println("DATA RECEIVED FROM VISION: ");
  Serial.print("detected0: ");
  Serial.print(detected0);
  Serial.print(", hori_dist0: ");
  Serial.print(hori_dist0);
  Serial.print(", diag_dist0: ");
  Serial.print(diag_dist0);
  Serial.print(", angle0 (rad): ");
  Serial.print(angle0);
  Serial.print(", angle0 (deg): ");
  Serial.println(angle0deg);

  Serial.print("detected1: ");
  Serial.print(detected1);
  Serial.print(", hori_dist1: ");
  Serial.print(hori_dist1);
  Serial.print(", diag_dist1: ");
  Serial.print(diag_dist1);
  Serial.print(", angle1 (rad): ");
  Serial.print(angle1);
  Serial.print(", angle1 (deg): ");
  Serial.println(angle1deg);

  Serial.print("detected2: ");
  Serial.print(detected2);
  Serial.print(", hori_dist2: ");
  Serial.print(hori_dist2);
  Serial.print(", diag_dist2: ");
  Serial.print(diag_dist2);
  Serial.print(", angle2 (rad): ");
  Serial.print(angle2);
  Serial.print(", angle2 (deg): ");
  Serial.println(angle2deg);

  Serial.print("detected3: ");
  Serial.print(detected3);
  Serial.print(", hori_dist3: ");
  Serial.print(hori_dist3);
  Serial.print(", diag_dist3: ");
  Serial.print(diag_dist3);
  Serial.print(", angle3 (rad): ");
  Serial.print(angle3);
  Serial.print(", angle3 (deg): ");
  Serial.println(angle3deg);

  Serial.print("detected4: ");
  Serial.print(detected4);
  Serial.print(", hori_dist4: ");
  Serial.print(hori_dist4);
  Serial.print(", diag_dist4: ");
  Serial.print(diag_dist4);
  Serial.print(", angle4 (rad): ");
  Serial.print(angle4);
  Serial.print(", angle4 (deg): ");
  Serial.println(angle4deg);
  delay(5000);
}
/*============================= END OF VISION FUNCTIONS =============================*/