#include <ControlLib2.h>

// UPDATE HERE ACCORDING TO YOUR WiFi Name, WiFi password, and Server's Public DNS Address
const char* ssid = "";                                            // your WiFi name
const char* password = "";                                        // your WiFi password
const char* mqtt_server = "ec2-18-223-15-156.us-east-2.compute.amazonaws.com";    // Server's Public DNS (get from COMMAND)

void setup() {
  Serial.begin(115200);         // for debugging, set up Serial Monitor for ESP32
  setupDrive();                 // set up Serial/UART Communication with DRIVE
  Wire.begin();                 // set up I2C connection with VISION's FPGA
  setup_wifi(ssid, password);   // connect ESP32 to WiFi
  setup_mqtt(mqtt_server);      // connect ESP32 to MQTT Broker
}

void loop() {
  MQTTconnect();                // receive data from COMMAND via MQTT
  sendToDrive();                // send data received from COMMAND to DRIVE via Serial/UART
  getDriveData();               // receive rover data from DRIVE via Serial/UART
  sendLiveLoc();                // send rover's live location to COMMAND via MQTT
  getVisionData();              // receive obstacle data from VISION via I2C
  getObstacleCoords();          // calculate obstacle coordinates based on data from DRIVE and VISION
  sendObstacleCoords();         // send obstacle data to COMMAND for mapping and DRIVE for obstacle avoidance
}
