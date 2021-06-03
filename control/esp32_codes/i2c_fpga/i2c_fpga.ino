#include <Wire.h>

// variables received from VISION
long BASE_ADDRESS = 0x40000;        // check platform designer in Quartus for BASE_ADDRESS of I2C_MEM
int8_t byte0, byte1, byte2, byte3;
int detected;                       // 1 only if obstacle is detected, 0 other wise
int angle;                          // angle of osbtacle to rover. on left of rover = +ve, on right of rover = -ve
int diag_dist;                      // diagonal distance of obstacle to rover

void setup() {
  Wire.begin();             // start I2C connection with vision's FPGA
  Serial.begin(115200);     // Serial Monitor for ESP32
}

void loop() {
  // receiving data from FPGA via I2C protocol
  getVisionDataI2C();
  getVisionData();
  printVisionData();
}

// receiving data from FPGA via I2C protocol
void getVisionDataI2C() {
  setAddress(BASE_ADDRESS);
  Wire.requestFrom(0x55, 4);    // request 1 word (4 bytes) from FPGA
  if (Wire.available() > 0) {
    byte0 = (Wire.read());
    byte1 = (Wire.read());
    byte2 = (Wire.read());
    byte3 = (Wire.read());
  }
}

// store values from VISION into variables
void getVisionData(){
  getDetected();
  getAngle();
  getDistance();
}
void getDetected(){
  detected = byte0;
}
void getAngle(){
  angle = byte1;
}
void getDistance(){
  if (angle < 0) {
    diag_dist = ((byte3<<8) + byte2) + 1;
  } else {
    diag_dist = ((byte3<<8) + byte2);
  }
}

// print out data received from VISION to serial monitor
void printVisionData() {
  if (detected > 0) {
    Serial.println("Data received from VISION: ");
    Serial.print("detected: ");
    Serial.println(detected);
    Serial.print("angle: ");
    Serial.println(angle);
    Serial.println("Diagonal distance to rover in mm: ");
    Serial.println(diag_dist);
    delay(5000);
  }
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
