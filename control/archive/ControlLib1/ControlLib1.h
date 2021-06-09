#ifndef CONTROL_LIB_H
#define CONTROL_LIB_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <math.h>
#include <Wire.h>

#define MSG_BUFFER_SIZE (50)

/*================================ COMMAND FUNCTIONS =================================*/
void setup_wifi(const char* ssid, const char* password);                     // connect ESP32 to WiFi
void callback(char* topic, byte* payload, unsigned int length);              // function to execute whenever a message is received for a subscribed topic from COMMAND
void setup_mqtt(const char* mqtt_server);                                    // set up to connect ESP32 to MQTT broker
void reconnect();                                                            // connect ESP32 to MQTT broker
void MQTTconnect();                                                          // receive data from COMMAND via MQTT
void parseCommandData(char* topic, char incomingData[MSG_BUFFER_SIZE]);      // parse data received from COMMAND and store into variables as needed
void printCommandData();                                                     // for debugging, function to print data received from COMMAND
void sendLiveLoc();                                                          // send rover coordinates to COMMAND only if there are changes to rover coordinates
void sendObstacleCoords();                                                   // send obstacle data to COMMAND for mapping and DRIVE for obstacle avoidance
void sendColor0Coords();
void sendColor1Coords();
void sendColor2Coords();
void sendColor3Coords();
void sendColor4Coords();
/*============================= END OF COMMAND FUNCTIONS =============================*/

/*================================= DRIVE FUNCTIONS ==================================*/
void setupDrive();              // set up Serial/UART connection with DRIVE
void getDriveData();            // receive rover data from DRIVE via Serial/UART
void sendToDrive();             // send data received from COMMAND to DRIVE via Serial/UART
void recvFromSerial2();         // receive data from Serial2 UART ports
void parseDriveData();          // parse data received from DRIVE and store values into variables
void printDriveData();          // for debugging, print data received from DRIVE to serial monitor
/*============================== END OF DRIVE FUNCTIONS ==============================*/

/*================================= VISION FUNCTIONS =================================*/
void setAddress(long addr);     // set the memory address on VISION's FPGA to read from for I2C communication
void getVisionData();           // receive obstacle data from VISION via I2C
void getVisionData0();
void getVisionData1();
void getVisionData2();
void getVisionData3();
void getVisionData4();
void printVisionData();         // for debugging, print out data received from VISION to serial monitor
/*============================= END OF VISION FUNCTIONS ==============================*/

/*======================= CALCULATION OF OBSTACLE COORDINATES ========================*/
void getSteerQuadrant();        // helper function for knowing which "quadrant" rover is in based on steering angle
void getCameraCoords();         // account for distance between rover camera and coordinate detection device
void printCameraCoords();       // for debugging, print camera coordinates to serial monitor
void getObstacleCoords();       // caculate x and y coordinates of obstacle only if obstacle has been detected
void getObstacleCoords0();
void getObstacleCoords1();
void getObstacleCoords2();
void getObstacleCoords3();
void getObstacleCoords4();
void printObstacleCoords();     // for debugging, print obstacle coordinates to serial monitor
/*==================== END OF CALCULATION OF OBSTACLE COORDINATES ====================*/

#endif