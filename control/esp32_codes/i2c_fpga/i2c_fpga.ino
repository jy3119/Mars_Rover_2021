#include <Wire.h>

// check platform designer in quartus for BASE_ADDRESS of I2C_MEM
long BASE_ADDRESS = 0x80000;

int obstacle_detected;    // 1 only if obstacle is detectd
int leftctr_or_right;    // 1 only if obstacle is to the right of rover, otherwise (left or straight ahead) 0
int hori_dist;            // horizontal distance of obstacle to rover
int diag_dist;            // diagonal distance of obstacle to rover

// helper function for I2C communication with vision's FPGA
void setAddress(long addr)  {
  Wire.beginTransmission(0x55);
  Wire.write(addr>>24);
  Wire.write((addr>>16)&255);
  Wire.write((addr>>8)&255);
  Wire.write(addr&255);
  Wire.endTransmission(false);
}

void setup()
{
  Wire.begin();           // I2C connection with vision's FPGA    
  Serial.begin(9600);     // Serial Monitor for ESP32
}

void loop()
{
  // receiving data from FPGA via I2C protocol
  setAddress(BASE_ADDRESS);     // read from BASE_ADDRESS of I2C_MEM 
  Wire.requestFrom(0x55, 4);    // request 1 word (4 bytes) from FPGA
  if (Wire.available() > 0) {
    obstacle_detected = Wire.read();
    leftctr_or_right = Wire.read();
    hori_dist = (Wire.read() * 10);
    diag_dist = (Wire.read() * 10);  

    // for debugging purposes, print out variables to serial monitor only if obstacle is detected.
    if (obstacle_detected > 0) {
      Serial.println("-----------------------------------------");
      Serial.print("obstacle_detected: ");
      Serial.print(obstacle_detected);
      Serial.println(" ");
      Serial.print("leftctr_or_right: ");
      Serial.print(leftctr_or_right);
      Serial.println(" ");
      Serial.print("Horizontal distance to rover in mm: ");
      Serial.print(hori_dist);
      Serial.println(" ");
      Serial.print("Diagonal distance to rover in mm: ");
      Serial.print(diag_dist);
      Serial.println(" ");
      Serial.println("-----------------------------------------");
      delay(10000);   
    }
  }
}
