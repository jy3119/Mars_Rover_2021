#ifndef CONTROL_LIB_3_H
#define CONTROL_LIB_3_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <math.h>
#include <WiFi.h>

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
void sendToDrive();             // send data received from COMMAND to DRIVE via Serial/UART
void recvFromSerial2();         // receive data from Serial2 UART ports
void parseDriveData();          // parse data received from DRIVE and store values into variables
void getDriveData();            // receive rover data from DRIVE via Serial/UART
void printDriveData();          // for debugging, print data received from DRIVE to serial monitor
/*============================== END OF DRIVE FUNCTIONS ==============================*/

/*================================= VISION FUNCTIONS =================================*/
void setupVision();              // set up Serial/UART connection with VISION
void printVisionData();         // for debugging, print out data received from VISION to serial monitor
void recvFromSerial1();         // receive data from Serial1 UART port
void parseVisionData();         // parse data received from VISION and store values into variables. also calculate angle of obstacles.
void getVisionData();           // receive obstacle data (color, detected, hori_dist, diag_dist) from VISION
void printVisionData();         // for debugging, print data received from VISION to serial monitor
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