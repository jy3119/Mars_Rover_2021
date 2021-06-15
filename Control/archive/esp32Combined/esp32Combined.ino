#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>
#include <Wire.h>

// for connecting ESP32 to wifi and MQTT server
// UPDATE HERE ACCORDING TO YOUR WIFI Name and password, and MQTT Server's Public DNS
const char* ssid = "AH LAM";
const char* password = "96258167";
const char* mqtt_server = "ec2-3-21-102-39.us-east-2.compute.amazonaws.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// ===================DECLARE ANY GLOBAL VARIABLES HERE===================

// Serial variables for UART communication
const byte numChars = 32;
boolean newData = false;
char receivedChars[numChars];

// MQTT variables for receiving messages on subcsribed topics
char incomingData[numChars];

// MQTT variables for publishing messages to topics
unsigned long currentTime;
unsigned long lastMsgTime = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

// variables received from COMMAND to send to DRIVE
int driveMode;
// automatic mode
int target_x;           // target x-coordinate to travel to
int target_y;           // target y-coordinate to travel to
int radius;             // radius to sweep in automatic mode
// manual mode: can only be used after all obstacles have been detected
int cmd_direction;      // forwards (0) or backwards(1)
int cmd_dist;           // travel distance in terms of mm
int cmd_angle;          // left or right
int cmd_speed;          // rover speed


// variables received from VISION
long BASE_ADDRESS = 0x80000;        // check platform designer in Quartus for BASE_ADDRESS of I2C_MEM
int8_t byte0, byte1, byte2, byte3;
int detected0, detected1, detected2, detected3, detected4;              // 1 only if obstacle is detected, 0 other wise
int angle0, angle1, angle2, angle3, angle4;                             // angle of osbtacle to rover. on left of rover = +ve, on right of rover = -ve. range: -90 to 90 degrees
int diag_dist0, diag_dist1, diag_dist2, diag_dist3, diag_dist4;         // diagonal distance of obstacle to rover


// variables received from DRIVE
int rover_x = 0;          // current x-coordinates of rover
int rover_y = 0;          // current y-coordinates of rover
int steeringAngle = 0;    // steering angle of rover, ACW +ve, CW -ve, range is -180 to 180 degrees

// variables for calculation of obstacle coords
int camera_x, camera_y;          // x and y coordinates of rover's front camera corrected for rover length
int roverCorrection = 220;       // correction for distance between coordinate detector and camera of rover
int steerQuadrant;
int dx0, dx1, dx2, dx3, dx4;     // send to DRIVE from CONTROL for automatic mode
int dy0, dy1, dy2, dy3, dy4;     // send to DRIVE from CONTROL for automatic mode

// variables to send to COMMAND for mapping, and also send obstacle coords to DRIVE
int color0_x, color1_x, color2_x, color3_x, color4_x;
int color0_y, color1_y, color2_y, color3_y, color4_y;
int x_last0, x_last1, x_last2, x_last3, x_last4;
int y_last0, y_last1, y_last2, y_last3, y_last4;


// ===========================================================================

void setup() {
  Serial.begin(115200);   // serial monitor for ESP32
  Wire.begin();           // start I2C connection with VISION's FPGA
  setup_wifi();           // connect ESP32 to WiFi
  setup_mqtt();           // connect ESP32 to MQTT Broker
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  getAllVisionData();         // receive data from VISION via I2C
  getAllObstacleCoords();     // calculate obstacle coordinates

  currentTime = millis();
  if (currentTime - lastMsgTime > 5000){
    lastMsgTime = currentTime;
    sendAllObstacleCoords();
  }
}

// ===========================================================================


//                            FUNCTION DEFINITIONS                            //
/*================================= COMMAND =================================*/
void sendAllObstacleCoords(){
  sendColor0Coords();
  sendColor1Coords();
  sendColor2Coords();
  sendColor3Coords();
  sendColor4Coords();
}

void sendColor0Coords(){
  if ((color0_x!=x_last0) || (color0_y!=y_last0)){
    x_last0 = color0_x;
    y_last0 = color0_y;
    if (detected0 == 1) {
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", color0_x, color0_y, 0);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("obstacle", msg);
    } else {}
  } else{}
}
void sendColor1Coords(){
  if ((color1_x!=x_last1) || (color1_y!=y_last1)){
    x_last1 = color1_x;
    y_last1 = color1_y;
    if (detected1 == 1) {
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", color1_x, color1_y, 1);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("obstacle", msg);
    } else {}
  } else{}
}
void sendColor2Coords(){
  if ((color2_x!=x_last2) || (color2_y!=y_last2)){
    x_last2 = color2_x;
    y_last2 = color2_y;
    if (detected2 == 1) {
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", color2_x, color2_y, 2);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("obstacle", msg);
    } else {}
  } else{}
}
void sendColor3Coords(){
  if ((color3_x!=x_last3) || (color3_y!=y_last3)){
    x_last3 = color3_x;
    y_last3 = color3_y;
    if (detected3 == 1) {
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", color3_x, color3_y, 3);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("obstacle", msg);
    } else {}
  } else{}
}
void sendColor4Coords(){
  if ((color4_x!=x_last4) || (color4_y!=y_last4)){
    x_last4 = color4_x;
    y_last4 = color4_y;
    if (detected4 == 1) {
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", color4_x, color4_y, 4);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("obstacle", msg);
    } else {}
  } else{}
}

// parse data received from COMMAND and store into variables as needed
void parseCommandData(char* topic, char incomingData[numChars]) {
  if (strcmp(topic, "auto") == 0) {
    char * strtokIndx;
    strtokIndx = strtok(incomingData, ",");
    target_x = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_y = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    radius = atoi(strtokIndx);
    driveMode = 0;
  }
  if (strcmp(topic, "manual") == 0) {
    char * strtokIndx;
    strtokIndx = strtok(incomingData, ",");
    cmd_direction = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_dist = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_angle = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_speed = atoi(strtokIndx);
    driveMode = 1;
  }
}

// function to execute whenever a message is received for a subscribed topic from COMMAND
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived for topic [");
  Serial.print(topic);
  Serial.println("] :");
  memset(incomingData, 0, numChars);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    incomingData[i] = (char)payload[i];
  }
  Serial.println();
  parseCommandData(topic, incomingData);
  printCommandData();     // print data received from COMMAND to serial monitor, for debugging
}

// connect ESP32 to MQTT broker
void reconnect() {
  // Loop until we're connected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Generating random Client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect. Once connected, publish an announcement
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish("outTopic", "Hello World from ESP32!");

      // SUBCSRIBE TO TOPICS HERE:
      client.subscribe("auto");
      client.subscribe("manual");

    } else {
      // MQTT connection failed
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// connect ESP32 to WiFi
void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// set up to connect ESP32 to MQTT broker
void setup_mqtt() {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// for debugging, function to print data received from COMMAND
void printCommandData() {
  if (driveMode == 0){
    Serial.println("Auto mode selected");
    Serial.print("0 for auto: ");
    Serial.println(driveMode);
    Serial.print("target_x: ");
    Serial.println(target_x);
    Serial.print("target_y: ");
    Serial.println(target_y);
    Serial.print("radius: ");
    Serial.println(radius);
  }
  if (driveMode == 1){
    Serial.println("Manual mode selected");
    Serial.print("1 for manual: ");
    Serial.println(driveMode);
    Serial.print("cmd_direction (0 for fwd, 1 for backwards): ");
    Serial.println(cmd_direction);
    Serial.print("cmd_dist: ");
    Serial.println(cmd_dist);
    Serial.print("cmd_angle: ");
    Serial.println(cmd_angle);
    Serial.print("cmd_speed: ");
    Serial.println(cmd_speed);
  }
}
/*================================= END OF COMMAND =================================*/

/*===================================== VISION =====================================*/
void getAllVisionData(){
  getVisionData0();
  getVisionData1();
  getVisionData2();
  getVisionData3();
  getVisionData4();
//  printVisionData();
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
/*================================= END OF VISION =================================*/

/*====================== CALCULATION OF OBSTACLE COORDINATES ======================*/
// caculate x and y coordinates of obstacle
void getAllObstacleCoords(){
  getObstacleCoords0();
  getObstacleCoords1();
  getObstacleCoords2();
  getObstacleCoords3();
  getObstacleCoords4();
//  printObstacleCoords();
}

// calculate coordinates of obstacles only if obstacle has been detected
void getObstacleCoords0() {
  if (detected0 == 1){
    getCameraCoords();
    float x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    int totalAngle = steeringAngle + angle0;
    float totalAngleRad = (totalAngle * 71) / 4068.0;
    x_dist_tmp = diag_dist0 * sin(totalAngleRad);
    y_dist_tmp = diag_dist0 * cos(totalAngleRad);
    getSteerQuadrant();
    if (totalAngle == 0) {
      x_dist = 0;
      y_dist = diag_dist0;
    } else if ((totalAngle == 180) || (totalAngle == 180)) {
      x_dist = 0;
      y_dist = -diag_dist0;
    } else if (totalAngle == 90) {
      x_dist = -diag_dist0;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = diag_dist0;
      y_dist = 0;
    } else {
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color0_x = camera_x + x_dist;
    color0_y = camera_y + y_dist;
    dx0 = x_dist;
    dy0 = y_dist;
  } else {}
}
void getObstacleCoords1() {
  if (detected1 == 1){
    getCameraCoords();
    float x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    int totalAngle = steeringAngle + angle1;
    float totalAngleRad = (totalAngle * 71) / 4068.0;
    x_dist_tmp = diag_dist1 * sin(totalAngleRad);
    y_dist_tmp = diag_dist1 * cos(totalAngleRad);
    getSteerQuadrant();
    if (totalAngle == 0) {
      x_dist = 0;
      y_dist = diag_dist1;
    } else if ((totalAngle == 180) || (totalAngle == 180)) {
      x_dist = 0;
      y_dist = -diag_dist1;
    } else if (totalAngle == 90) {
      x_dist = -diag_dist1;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = diag_dist1;
      y_dist = 0;
    } else {
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color1_x = camera_x + x_dist;
    color1_y = camera_y + y_dist;
    dx1 = x_dist;
    dy1 = y_dist;
  } else {}
}
void getObstacleCoords2() {
  if (detected2 == 1){
    getCameraCoords();
    float x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    int totalAngle = steeringAngle + angle2;
    float totalAngleRad = (totalAngle * 71) / 4068.0;
    x_dist_tmp = diag_dist2 * sin(totalAngleRad);
    y_dist_tmp = diag_dist2 * cos(totalAngleRad);
    getSteerQuadrant();
    if (totalAngle == 0) {
      x_dist = 0;
      y_dist = diag_dist2;
    } else if ((totalAngle == 180) || (totalAngle == 180)) {
      x_dist = 0;
      y_dist = -diag_dist2;
    } else if (totalAngle == 90) {
      x_dist = -diag_dist2;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = diag_dist2;
      y_dist = 0;
    } else {
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color2_x = camera_x + x_dist;
    color2_y = camera_y + y_dist;
    dx2 = x_dist;
    dy2 = y_dist;
  } else {}
}
void getObstacleCoords3() {
  if (detected3 == 1){
    getCameraCoords();
    float x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    int totalAngle = steeringAngle + angle3;
    float totalAngleRad = (totalAngle * 71) / 4068.0;
    x_dist_tmp = diag_dist3 * sin(totalAngleRad);
    y_dist_tmp = diag_dist3 * cos(totalAngleRad);
    getSteerQuadrant();
    if (totalAngle == 0) {
      x_dist = 0;
      y_dist = diag_dist3;
    } else if ((totalAngle == 180) || (totalAngle == 180)) {
      x_dist = 0;
      y_dist = -diag_dist3;
    } else if (totalAngle == 90) {
      x_dist = -diag_dist3;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = diag_dist3;
      y_dist = 0;
    } else {
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color3_x = camera_x + x_dist;
    color3_y = camera_y + y_dist;
    dx3 = x_dist;
    dy3 = y_dist;
  } else {}
}
void getObstacleCoords4() {
  if (detected4 == 1){
    getCameraCoords();
    float x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    int totalAngle = steeringAngle + angle4;
    float totalAngleRad = (totalAngle * 71) / 4068.0;
    x_dist_tmp = diag_dist4 * sin(totalAngleRad);
    y_dist_tmp = diag_dist4 * cos(totalAngleRad);
    getSteerQuadrant();
    if (totalAngle == 0) {
      x_dist = 0;
      y_dist = diag_dist4;
    } else if ((totalAngle == 180) || (totalAngle == 180)) {
      x_dist = 0;
      y_dist = -diag_dist4;
    } else if (totalAngle == 90) {
      x_dist = -diag_dist4;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = diag_dist4;
      y_dist = 0;
    } else {
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color4_x = camera_x + x_dist;
    color4_y = camera_y + y_dist;
    dx4 = x_dist;
    dy4 = y_dist;
  } else {}
}

// account for distance between rover camera and coordinate detection device
void getCameraCoords() {
  float x_dist_tmp, y_dist_tmp, x_dist, y_dist;
  float steeringAngleRad = (steeringAngle * 71) / 4068.0;
  x_dist_tmp = roverCorrection * sin(steeringAngleRad);
  y_dist_tmp = roverCorrection * cos(steeringAngleRad);
  getSteerQuadrant();
  switch (steerQuadrant) {
    case 1: // steering angle = 0, i.e. rover is facing straight ahead
      x_dist = 0;
      y_dist = roverCorrection;
      break;
    case 2: // steering angle between 0 and 90 degrees: x -ve, y +ve
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 3: // steering angle = 90, i.e. rover is facing 90 degrees to the left / ACW
      x_dist = -roverCorrection;
      y_dist = 0;
      break;
    case 4: // steering angle between 90 and 180 degrees: x -ve, y -ve
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 5: // steering angle = 180 or -180, i.e. rover is facing straight behind
      x_dist = 0;
      y_dist = -roverCorrection;
      break;
    case 6: // steering angle between 0 and -90 degrees: x +ve, y +ve
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 7: // steering angle = -90, i.e. rover is facing 90 degrees to the right / CW
      x_dist = roverCorrection;
      y_dist = 0;
      break;
    case 8: // steering angle between -90 and -180 degrees: x +ve, y -ve
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
  }
  camera_x = rover_x + x_dist;
  camera_y = rover_y + y_dist;
//  printCameraCoords(); // print to serial monitor for debugging purposes
}

// helper function for getting camera coordinates
void getSteerQuadrant() {
  if (steeringAngle == 0) {
    steerQuadrant = 1;
  } else if ((steeringAngle > 0) && (steeringAngle < 90)) {
    steerQuadrant = 2;
  } else if (steeringAngle == 90) {
    steerQuadrant = 3;
  } else if ((steeringAngle > 90) && (steeringAngle < 180)) {
    steerQuadrant = 4;
  } else if ((steeringAngle == 180) || (steeringAngle == -180)) {
    steerQuadrant = 5;
  } else if ((steeringAngle < 0) && (steeringAngle > -90)) {
    steerQuadrant = 6;
  } else if (steeringAngle == -90) {
    steerQuadrant = 7;
  } else if ((steeringAngle < -90) && (steeringAngle > -180)) {
    steerQuadrant = 8;
  }
}

// print obstacle coordinates to serial monitor, for debugging
void printObstacleCoords() {
  Serial.print("color0 x: ");
  Serial.print(color0_x);
  Serial.print(", color0 y: ");
  Serial.println(color0_y);
  
  Serial.print("color1 x: ");
  Serial.print(color1_x);
  Serial.print(", color1 y: ");
  Serial.println(color1_y);
  
  Serial.print("color2 x: ");
  Serial.print(color2_x);
  Serial.print(", color2 y: ");
  Serial.println(color2_y);
  
  Serial.print("color3 x: ");
  Serial.print(color3_x);
  Serial.print(", color3 y: ");
  Serial.println(color3_y);
  
  Serial.print("color4 x: ");
  Serial.print(color4_x);
  Serial.print(", color4 y: ");
  Serial.println(color4_y);
}
// print camera coordinates to serial monitor, for debugging
void printCameraCoords() {
  Serial.print("x-coordinates of camera: ");
  Serial.println(camera_x);
  Serial.print("y-coordinates of camera: ");
  Serial.println(camera_y);
}
/*================== END OF CALCULATION OF OBSTACLE COORDINATES ==================*/

// receive what's written to serial monitor
void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (Serial.available() > 0) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
