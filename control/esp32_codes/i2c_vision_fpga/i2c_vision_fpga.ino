#include <Wire.h>

// variables received from VISION
long BASE_ADDRESS = 0x80000;        // check platform designer in Quartus for BASE_ADDRESS of I2C_MEM
int8_t byte0, byte1, byte2, byte3;
int detected0, detected1, detected2, detected3, detected4;           // 1 only if obstacle is detected, 0 other wise
int angle0, angle1, angle2, angle3, angle4;                          // angle of osbtacle to rover. on left of rover = +ve, on right of rover = -ve
int diag_dist0, diag_dist1, diag_dist2, diag_dist3, diag_dist4;      // diagonal distance of obstacle to rover

void setup() {
  Wire.begin();             // start I2C connection with vision's FPGA
  Serial.begin(115200);     // Serial Monitor for ESP32
}

void loop() {
  // receiving data from FPGA via I2C protocol
  getAllVisionData();
}

void getAllVisionData(){
  getVisionData0();
  getVisionData1();
  getVisionData2();
  getVisionData3();
  getVisionData4();
  printVisionData();
}

// receiving data from FPGA via I2C protocol
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
    angle0 = byte1;
    if (angle0 < 0) {
      diag_dist0 = (((byte3 << 8) + byte2) + 1) * 10;
    } else {
      diag_dist0 = ((byte3 << 8) + byte2) * 10;
    }
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
    angle1 = byte1;
    if (angle1 < 0) {
      diag_dist1 = (((byte3 << 8) + byte2) + 1) * 10;
    } else {
      diag_dist1 = ((byte3 << 8) + byte2) * 10;
    }
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
    angle2 = byte1;
    if (angle2 < 0) {
      diag_dist2 = (((byte3 << 8) + byte2) + 1) * 10;
    } else {
      diag_dist2 = ((byte3 << 8) + byte2) * 10;
    }
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
    angle3 = byte1;
    if (angle3 < 0) {
      diag_dist3 = (((byte3 << 8) + byte2) + 1) * 10;
    } else {
      diag_dist3 = ((byte3 << 8) + byte2) * 10;
    }
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
    angle4 = byte1;
    if (angle4 < 0) {
      diag_dist4 = (((byte3 << 8) + byte2) + 1) * 10;
    } else {
      diag_dist4 = ((byte3 << 8) + byte2) * 10;
    }
  }
}


// print out data received from VISION to serial monitor
void printVisionData() {
  Serial.println("DATA RECEIVED FROM VISION: ");
  Serial.print("detected0: ");
  Serial.print(detected0);
  Serial.print(", angle0: ");
  Serial.print(angle0);
  Serial.print(", diag_dist0: ");
  Serial.println(diag_dist0);

  Serial.print("detected1: ");
  Serial.print(detected1);
  Serial.print(", angle1: ");
  Serial.print(angle1);
  Serial.print(", diag_dist1: ");
  Serial.println(diag_dist1);

  Serial.print("detected2: ");
  Serial.print(detected1);
  Serial.print(", angle2: ");
  Serial.print(angle2);
  Serial.print(", diag_dist2: ");
  Serial.println(diag_dist2);

  Serial.print("detected3: ");
  Serial.print(detected3);
  Serial.print(", angle3: ");
  Serial.print(angle3);  
  Serial.print(", diag_dist3: ");
  Serial.println(diag_dist3);

  Serial.print("detected4: ");
  Serial.print(detected4);
  Serial.print(", angle4: ");
  Serial.print(angle4);
  Serial.print(", diag_dist4: ");
  Serial.println(diag_dist4);
  delay(5000);
}

// helper function for I2C communication with vision's FPGA
void setAddress(long addr)  {
  Wire.beginTransmission(0x55);
  Wire.write(addr >> 24);
  Wire.write((addr >> 16) & 255);
  Wire.write((addr >> 8) & 255);
  Wire.write(addr & 255);
  Wire.endTransmission(false);
}
