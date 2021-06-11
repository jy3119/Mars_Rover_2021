#include <ControlLib3.h>

/* ====================================== DECLARE ANY GLOBAL VARIABLES HERE ====================================== */
// MQTT variables for connecting to MQTT Broker
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);
// MQTT variables for receiving messages on subcsribed topics from COMMAND
#define MSG_BUFFER_SIZE (50)
char incomingData[MSG_BUFFER_SIZE];
// MQTT variables for publishing messages to COMMAND
char msg[MSG_BUFFER_SIZE];
char msgLiveLoc[MSG_BUFFER_SIZE];

// variables received from COMMAND to send to DRIVE
int cmdID, prevID;              // ID of message received from COMMAND. helper variable
char driveMode, prevMode;       // mode = 'A' for auto, 'M' for manual  
// automatic mode
int target_x;                   // target x-coordinate to travel to
int target_y;                   // target y-coordinate to travel to
int radius;                     // radius to sweep in automatic mode
// manual mode: can only be used after all obstacles have been detected
int cmd_direction;              // 0 for forwards, 1 for backwards
int cmd_dist;                   // travel distance in terms of mm
int cmd_angle;                  // -180 to 180, ACW is positive, CW is negative
int cmd_speed;                  // rover speed


// Serial variables for UART/Serial communication with DRIVE
#define TXD2 17 // look for port 8 on the arduino adaptor of the ESP32, connect this to RX on the arduino
#define RXD2 16 // look for port 9 on the arduino adaptor of the ESP32, connect this to TX on the arduino
char msgDrive[MSG_BUFFER_SIZE];
char msgObst[MSG_BUFFER_SIZE];
char recvFromDrive[MSG_BUFFER_SIZE];
char tmpFromDrive[MSG_BUFFER_SIZE];
boolean newData = false;
// variables received from DRIVE
int rover_x;                     // current x-coordinates of rover
int rover_y;                     // current y-coordinates of rover
int steeringAngle;               // steering angle of rover, ACW +ve, CW -ve, range is -180 to 180 degrees
double steeringAngleRad;
int roverPrevX, roverPrevY, prevSteerAngle;


// Serial variables for UART/Serial communication with VISION
#define RXD1 21
#define TXD1 22
char recvFromVision[MSG_BUFFER_SIZE];
char tmpFromVision[MSG_BUFFER_SIZE];
boolean newDataV = false;
int color;
int detected0, detected1, detected2, detected3, detected4;              // 1 only if obstacle is detected, 0 other wise
int diag_dist0, diag_dist1, diag_dist2, diag_dist3, diag_dist4;         // diagonal distance of obstacle to rover, received in mm
int hori_dist0, hori_dist1, hori_dist2, hori_dist3, hori_dist4;         // horizontal distance of obstacle to rover, received in mm
double ratio0, ratio1, ratio2, ratio3, ratio4;                          // ratio of hori_dist / diag_dist
double angle0, angle1, angle2, angle3, angle4;                          // angle of osbtacle to rover. on left of rover = +ve,  on right of rover = -ve.  range: -90 to 90 degrees
double angle0deg, angle1deg, angle2deg, angle3deg, angle4deg;

// variables for calculation of obstacle coords
int camera_x, camera_y;          // x and y coordinates of rover's front camera corrected for rover length
long roverCorrection = 220;      // correction for distance between coordinate detector and camera of rover
int steerQuadrant;
int dx0, dx1, dx2, dx3, dx4;     // send to DRIVE 
int dy0, dy1, dy2, dy3, dy4;     // send to DRIVE 

// variables to send to COMMAND for mapping, and also send obstacle coords to DRIVE
long color0_x = 10000, color1_x = 10000, color2_x = 10000, color3_x = 10000, color4_x = 10000;
long color0_y = 10000, color1_y = 10000, color2_y = 10000, color3_y = 10000, color4_y = 10000;
long x_last0 = 10000, x_last1 = 10000, x_last2 = 10000, x_last3 = 10000, x_last4 = 10000;
long y_last0 = 10000, y_last1 = 10000, y_last2 = 10000, y_last3 = 10000, y_last4 = 10000;


/* ============================================= FUNCTION DEFINITIONS ============================================= */
/*================================ COMMAND FUNCTIONS =================================*/
// connect ESP32 to WiFi
void setup_wifi(const char* ssid, const char* password) {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// function to execute whenever a message is received for a subscribed topic from COMMAND
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("From COMMAND: Message arrived for topic ["); // for debugging
  Serial.print(topic); // for debugging
  Serial.print("] : "); // for debugging
  memset(incomingData, 0, MSG_BUFFER_SIZE);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    incomingData[i] = (char)payload[i];
  }
  Serial.println();
  parseCommandData(topic, incomingData);
  printCommandData();     // for debugging
}

// set up to connect ESP32 to MQTT broker
void setup_mqtt(const char* mqtt_server) {
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
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

// receive data from COMMAND via MQTT
void MQTTconnect() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();              
}

// parse data received from COMMAND and store into variables as needed
void parseCommandData(char* topic, char incomingData[MSG_BUFFER_SIZE]) {
  if (strcmp(topic, "auto") == 0) {
    char * strtokIndx;
    strtokIndx = strtok(incomingData, ",");
    cmdID = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_x = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_y = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    radius = atoi(strtokIndx);
    driveMode = 'A';
  }
  if (strcmp(topic, "manual") == 0) {
    char * strtokIndx;
    strtokIndx = strtok(incomingData, ",");
    cmdID = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_direction = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_dist = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_angle = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_speed = atoi(strtokIndx);
    driveMode = 'M';
  }
}

// for debugging, function to print data received from COMMAND
void printCommandData() {
  Serial.println("The following information is printed by printCommandData function: ");
  Serial.print("Instruction ID: ");  
  Serial.println(cmdID);
  if (driveMode == 'A'){
    Serial.println("Auto mode selected");
    Serial.print("driveMode: ");
    Serial.println(driveMode);
    Serial.print("target_x: ");
    Serial.println(target_x);
    Serial.print("target_y: ");
    Serial.println(target_y);
    Serial.print("radius: ");
    Serial.println(radius);
    delay(5000);
  }
  if (driveMode == 'M'){
    Serial.println("Manual mode selected");
    Serial.print("driveMode: ");
    Serial.println(driveMode);
    Serial.print("cmd_direction (0 for fwd, 1 for backwards): ");
    Serial.println(cmd_direction);
    Serial.print("cmd_dist: ");
    Serial.println(cmd_dist);
    Serial.print("cmd_angle: ");
    Serial.println(cmd_angle);
    Serial.print("cmd_speed: ");
    Serial.println(cmd_speed);
    delay(5000);
  }
}

// send rover coordinates to COMMAND only if there are changes to rover coordinates
void sendLiveLoc(){
  if ((rover_x!=roverPrevX) || (rover_y!=roverPrevY) || (steeringAngle!=prevSteerAngle)) {
    roverPrevX = rover_x;
    roverPrevY = rover_y;
    prevSteerAngle = steeringAngle;
    snprintf (msgLiveLoc, MSG_BUFFER_SIZE, "%i,%i,%i", rover_x, rover_y, steeringAngle);
    Serial.print("To COMMAND: Rover's current coordinates and steering angle: "); // for debugging
    Serial.println(msgLiveLoc); // for debugging
    client.publish("liveloc", msgLiveLoc);
  }
}

// send obstacle data to COMMAND for mapping and DRIVE for obstacle avoidance
void sendObstacleCoords(){
  sendColor0Coords();
  sendColor1Coords();
  sendColor2Coords();
  sendColor3Coords();
  sendColor4Coords();
}
void sendColor0Coords(){
  // account for uncertainty value of diameter of a ping pong ball
  if ( (abs(color0_x-x_last0)>40) || (abs(color0_y-y_last0)>40) ) {
    x_last0 = color0_x;
    y_last0 = color0_y;
    if (detected0 == 1) {
      Serial.println("From VISION: RED Obstacle detected.");
      // to COMMAND
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", 0, color0_x, color0_y);
      client.publish("obstacle", msg);
      Serial.print("To COMMAND: obstacle's colour, x coords and y coords: "); // for debugging
      Serial.println(msg);   // for debugging
      // to DRIVE
      snprintf (msgObst, MSG_BUFFER_SIZE, "<%c,%i,%i,%i,%i,%i>",'O', 0, color0_x, color0_y, dx0, dy0);
      Serial2.write(msgObst);
      Serial.print("To DRIVE: obstacle coordinates(x,y) and distance (dx and dy) data: "); // for debugging
      Serial.println(msgObst); // for debugging
    } else {}
  } else{}
}
void sendColor1Coords(){
  if ( (abs(color1_x-x_last1)>40) || (abs(color1_y-y_last1)>40) ) {
    x_last1 = color1_x;
    y_last1 = color1_y;
    if (detected1 == 1) {
      Serial.println("From VISION: GREEN Obstacle detected.");
      // to COMMAND
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", 1, color1_x, color1_y);
      client.publish("obstacle", msg);
      Serial.print("To COMMAND: obstacle's colour, x coords and y coords: "); // for debugging
      Serial.println(msg);   // for debugging
      // to DRIVE
      snprintf (msgObst, MSG_BUFFER_SIZE, "<%c,%i,%i,%i,%i,%i>",'O', 1, color1_x, color1_y, dx1, dy1);
      Serial2.write(msgObst);
      Serial.print("To DRIVE: obstacle coordinates(x,y) and distance (dx and dy) data: "); // for debugging
      Serial.println(msgObst); // for debugging
    } else {}
  } else{}
}
void sendColor2Coords(){
  if ( (abs(color2_x-x_last2)>40) || (abs(color2_y-y_last2)>40) ) {
    x_last2 = color2_x;
    y_last2 = color2_y;
    if (detected2 == 1) {
      Serial.println("From VISION: BLUE Obstacle detected.");
      // to COMMAND
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", 2, color2_x, color2_y);
      client.publish("obstacle", msg);
      Serial.print("To COMMAND: obstacle's colour, x coords and y coords: "); // for debugging
      Serial.println(msg);   // for debugging
      // to DRIVE
      snprintf (msgObst, MSG_BUFFER_SIZE, "<%c,%i,%i,%i,%i,%i>",'O', 2, color2_x, color2_y, dx2, dy2);
      Serial2.write(msgObst);
      Serial.print("To DRIVE: obstacle coordinates(x,y) and distance (dx and dy) data: "); // for debugging
      Serial.println(msgObst); // for debugging
    } else {}
  } else{}
}
void sendColor3Coords(){
  if ( (abs(color3_x-x_last3)>40) || (abs(color3_y-y_last3)>40) ) {
    x_last3 = color3_x;
    y_last3 = color3_y;
    if (detected3 == 1) {
      Serial.println("From VISION: PINK Obstacle detected.");
      // to COMMAND
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", 3, color3_x, color3_y);
      client.publish("obstacle", msg);
      Serial.print("To COMMAND: obstacle's colour, x coords and y coords: "); // for debugging
      Serial.println(msg);   // for debugging
      // to DRIVE
      snprintf (msgObst, MSG_BUFFER_SIZE, "<%c,%i,%i,%i,%i,%i>",'O', 3, color3_x, color3_y, dx3, dy3);
      Serial2.write(msgObst);
      Serial.print("To DRIVE: obstacle coordinates(x,y) and distance (dx and dy) data: "); // for debugging
      Serial.println(msgObst); // for debugging
    } else {}
  } else{}
}
void sendColor4Coords(){
  if ( (abs(color4_x-x_last4)>40) || (abs(color4_y-y_last4)>40) ) {
    x_last4 = color4_x;
    y_last4 = color4_y;
    if (detected4 == 1) {
      Serial.println("From VISION: YELLOW Obstacle detected.");
      // to COMMAND
      snprintf (msg, MSG_BUFFER_SIZE, "%i,%i,%i", 4, color4_x, color4_y);
      client.publish("obstacle", msg);
      Serial.print("To COMMAND: obstacle's colour, x coords and y coords: "); // for debugging
      Serial.println(msg);   // for debugging
      // to DRIVE
      snprintf (msgObst, MSG_BUFFER_SIZE, "<%c,%i,%i,%i,%i,%i>",'O', 4, color4_x, color4_y, dx4, dy4);
      Serial2.write(msgObst);
      Serial.print("To DRIVE: obstacle coordinates(x,y) and distance (dx and dy) data: "); // for debugging
      Serial.println(msgObst); // for debugging
    } else {}
  } else{}
}
/*============================= END OF COMMAND FUNCTIONS =============================*/

/*================================= DRIVE FUNCTIONS ==================================*/
// set up Serial/UART connection with DRIVE
void setupDrive(){
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

// send data received from COMMAND to DRIVE via Serial/UART
void sendToDrive() {
  if ( (driveMode != prevMode) || ((driveMode == prevMode)&&(cmdID != prevID)) ){
    prevMode = driveMode;
    prevID = cmdID;
    // automatic mode
    if (driveMode == 'A') {
      snprintf (msgDrive, MSG_BUFFER_SIZE, "<%c,%i,%i,%i>", driveMode, target_x, target_y, radius);
    }
    // manual mode
    if (driveMode == 'M') {
      snprintf (msgDrive, MSG_BUFFER_SIZE, "<%c,%i,%i,%i,%i>", driveMode, cmd_direction, cmd_dist, cmd_angle, cmd_speed);
    }
    Serial2.write(msgDrive);
    Serial.print("To DRIVE: instruction mode and values from COMMAND: "); // for debugging
    Serial.println(msgDrive); // for debugging
  } else {}
}

// receive data from Serial2 UART ports
void recvFromSerial2() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial2.available() > 0 && newData == false) {
    rc = Serial2.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        recvFromDrive[ndx] = rc;
        ndx++;
        if (ndx >= MSG_BUFFER_SIZE) {
          ndx = MSG_BUFFER_SIZE - 1;
        }
      }
      else {
        recvFromDrive[ndx] = '\0'; // terminate the string
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

// parse data received from DRIVE and store values into variables
void parseDriveData() {
  char * strtokIndx;
  strtokIndx = strtok(recvFromDrive, ",");
  rover_x = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  rover_y = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  steeringAngle = atoi(strtokIndx);
  steeringAngleRad = (steeringAngle * 71) / 4068.0;
}

// get rover data (coordinates, steering angle) from DRIVE
void getDriveData() {
  recvFromSerial2();
  if (newData == true) {
    Serial.print("From DRIVE: rover coordinates and steering angle: "); // for debugging
    Serial.println(recvFromDrive); // for debugging
    strcpy(tmpFromDrive, recvFromDrive);
    parseDriveData();
    printDriveData();    // for debugging
    newData = false;
  }
}

// for debugging, print data received from DRIVE to serial monitor
void printDriveData() {
  Serial.println("The following information is printed by printDriveData function: ");
  Serial.print("rover_x: ");
  Serial.println(rover_x);
  Serial.print("rover_y: ");
  Serial.println(rover_y);
  Serial.print("steeringAngle (deg): ");
  Serial.print(steeringAngle);
  Serial.print(", steeringAngle (rad): ");
  Serial.println(steeringAngleRad);
  delay(5000);
}
/*============================== END OF DRIVE FUNCTIONS ==============================*/

/*================================= VISION FUNCTIONS =================================*/
// set up Serial/UART connection with VISION
void setupVision() {
  Serial1.begin(57600, SERIAL_8N1, RXD1, TXD1);
}

// receive data from Serial1 UART port
void recvFromSerial1() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newDataV == false) {
    rc = Serial1.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        recvFromVision[ndx] = rc;
        ndx++;
        if (ndx >= MSG_BUFFER_SIZE) {
          ndx = MSG_BUFFER_SIZE - 1;
        }
      }
      else {
        recvFromVision[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newDataV = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

// parse data received from VISION and store values into variables. also calculate angle of obstacles.
void parseVisionData() {
  char * strtokIndx;
  strtokIndx = strtok(recvFromVision, ",");
  color = atoi(strtokIndx);
  switch (color) {
    case 0:
      strtokIndx = strtok(NULL, ",");
      detected0 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist0 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist0 = atoi(strtokIndx);
      ratio0 = (double)hori_dist0 / (double)diag_dist0;
      angle0 = asin(ratio0); 
      angle0deg = (angle0 * 4068) / 71.0;
      break;
    case 1:
      strtokIndx = strtok(NULL, ",");
      detected1 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist1 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist1 = atoi(strtokIndx);
      ratio1 = (double)hori_dist1 / (double)diag_dist1;
      angle1 = asin(ratio1); 
      angle1deg = (angle1 * 4068) / 71.0;
      break;
    case 2:
      strtokIndx = strtok(NULL, ",");
      detected2 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist2 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist2 = atoi(strtokIndx);
      ratio2 = (double)hori_dist2 / (double)diag_dist2;
      angle2 = asin(ratio2); 
      angle2deg = (angle2 * 4068) / 71.0;
      break;
    case 3:
      strtokIndx = strtok(NULL, ",");
      detected3 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist3 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist3 = atoi(strtokIndx);
      ratio3 = (double)hori_dist3 / (double)diag_dist3;
      angle3 = asin(ratio3); 
      angle3deg = (angle3 * 4068) / 71.0;
      break;
    case 4:
      strtokIndx = strtok(NULL, ",");
      detected4 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist4 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist4 = atoi(strtokIndx);
      ratio4 = (double)hori_dist4 / (double)diag_dist4;
      angle4 = asin(ratio4); 
      angle4deg = (angle4 * 4068) / 71.0;
      break;
  }
}

// get obstacle data (color, detected, hori_dist, diag_dist) from VISION
void getVisionData() {
  recvFromSerial1();
  if (newDataV == true) {
    Serial.print("From VISION: Obstacle color, detected, hori_dist, diag_dist: "); // for debugging
    Serial.println(recvFromVision); // for debugging
    strcpy(tmpFromVision, recvFromVision);
    parseVisionData();
    printVisionData();    // for debugging
    newDataV = false;
  }
}

// for debugging, print data received from VISION to serial monitor
void printVisionData() {
  Serial.println("The following information is printed by printVisionData function: ");
  switch (color) {
    case 0:
      Serial.println("From VISION: RED Obstacle detected.");
      Serial.print("detected0: ");
      Serial.print(detected0);
      Serial.print(", hori_dist0: ");
      Serial.print(hori_dist0);
      Serial.print(", diag_dist0:  ");
      Serial.println(diag_dist0);
      Serial.print("angle0 (rad): ");
      Serial.print(angle0);
      Serial.print(", angle0 (deg): ");
      Serial.println(angle0deg);
      break;
    case 1:
      Serial.println("From VISION: GREEN Obstacle detected.");
      Serial.print("detected1: ");
      Serial.print(detected1);
      Serial.print(", hori_dist1: ");
      Serial.print(hori_dist1);
      Serial.print(", diag_dist1:  ");
      Serial.println(diag_dist1);
      Serial.print("angle1 (rad): ");
      Serial.print(angle1);
      Serial.print(", angle1 (deg): ");
      Serial.println(angle1deg);
      break;
    case 2:
      Serial.println("From VISION: BLUE Obstacle detected.");
      Serial.print("detected2: ");
      Serial.print(detected2);
      Serial.print(", hori_dist2: ");
      Serial.print(hori_dist2);
      Serial.print(", diag_dist2:  ");
      Serial.println(diag_dist2);
      Serial.print("angle2 (rad): ");
      Serial.print(angle2);
      Serial.print(", angle2 (deg): ");
      Serial.println(angle2deg);
      break;
    case 3:
      Serial.println("From VISION: PINK Obstacle detected.");
      Serial.print("detected3: ");
      Serial.print(detected3);
      Serial.print(", hori_dist3: ");
      Serial.print(hori_dist3);
      Serial.print(", diag_dist3:  ");
      Serial.println(diag_dist3);
      Serial.print("angle3 (rad): ");
      Serial.print(angle3);
      Serial.print(", angle3 (deg): ");
      Serial.println(angle3deg);
      break;
    case 4:
      Serial.println("From VISION: YELLOW Obstacle detected.");
      Serial.print("detected4: ");
      Serial.print(detected4);
      Serial.print(", hori_dist4: ");
      Serial.print(hori_dist4);
      Serial.print(", diag_dist4:  ");
      Serial.println(diag_dist4);
      Serial.print("angle4 (rad): ");
      Serial.print(angle4);
      Serial.print(", angle4 (deg): ");
      Serial.println(angle4deg);
      break;
  }
}
/*============================= END OF VISION FUNCTIONS ==============================*/

/*======================= CALCULATION OF OBSTACLE COORDINATES ========================*/
// helper function for knowing which "quadrant" rover is in based on steering angle
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

// account for distance between rover camera and coordinate detection device
void getCameraCoords() {
  double x_dist_tmp, y_dist_tmp, x_dist, y_dist;
  x_dist_tmp = roverCorrection * sin(steeringAngleRad);
  y_dist_tmp = roverCorrection * cos(steeringAngleRad);
  getSteerQuadrant();
  switch (steerQuadrant) {
    case 1: // steering angle = 0, i.e. rover is facing straight ahead
      x_dist = 0;
      y_dist = roverCorrection;
      break;
    case 2: // steering angle between 0 and 90 degrees: x +ve, y +ve
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 3: // steering angle = 90, i.e. rover is facing 90 degrees to the left / ACW
      x_dist = roverCorrection;
      y_dist = 0;
      break;
    case 4: // steering angle between 90 and 180 degrees: x +ve, y -ve
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 5: // steering angle = 180 or -180, i.e. rover is facing straight behind
      x_dist = 0;
      y_dist = -roverCorrection;
      break;
    case 6: // steering angle between 0 and -90 degrees: x -ve, y +ve
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 7: // steering angle = -90, i.e. rover is facing 90 degrees to the right / CW
      x_dist = -roverCorrection;
      y_dist = 0;
      break;
    case 8: // steering angle between -90 and -180 degrees: x -ve, y -ve
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
  }
  camera_x = rover_x + x_dist;
  camera_y = rover_y + y_dist;
//  printCameraCoords();    // for debugging
}

// for debugging, print camera coordinates to serial monitor
void printCameraCoords() {
  Serial.print("x-coordinates of camera: ");
  Serial.println(camera_x);
  Serial.print("y-coordinates of camera: ");
  Serial.println(camera_y);
  delay(5000);
}

// caculate x and y coordinates of obstacle only if obstacle has been detected
void getObstacleCoords(){
  getObstacleCoords0();
  getObstacleCoords1();
  getObstacleCoords2();
  getObstacleCoords3();
  getObstacleCoords4();
//  printObstacleCoords();      // for debugging
}
void getObstacleCoords0() {
  if (detected0 == 1){
    getCameraCoords();
    double x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    double totalAngleRad = steeringAngleRad + angle0;
    int totalAngle = steeringAngle + angle0deg;
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
      x_dist = diag_dist0;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = -diag_dist0;
      y_dist = 0;
    } else {
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color0_x = camera_x + x_dist;
    color0_y = camera_y + y_dist;
    dx0 = hori_dist0;
    dy0 = sqrt(sq(diag_dist0)-sq(hori_dist0));
  } else {}
}
void getObstacleCoords1() {
  if (detected1 == 1){
    getCameraCoords();
    double x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    double totalAngleRad = steeringAngleRad + angle1;
    int totalAngle = steeringAngle + angle1deg;
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
      x_dist = diag_dist1;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = -diag_dist1;
      y_dist = 0;
    } else {
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color1_x = camera_x + x_dist;
    color1_y = camera_y + y_dist;
    dx1 = hori_dist1;
    dy1 = sqrt(sq(diag_dist1)-sq(hori_dist1));
  } else {}
}
void getObstacleCoords2() {
  if (detected2 == 1){
    getCameraCoords();
    double x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    double totalAngleRad = steeringAngleRad + angle2;
    int totalAngle = steeringAngle + angle2deg;
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
      x_dist = diag_dist2;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = -diag_dist2;
      y_dist = 0;
    } else {
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color2_x = camera_x + x_dist;
    color2_y = camera_y + y_dist;
    dx2 = hori_dist2;
    dy2 = sqrt(sq(diag_dist2)-sq(hori_dist2));
  } else {}
}
void getObstacleCoords3() {
  if (detected3 == 1){
    getCameraCoords();
    double x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    double totalAngleRad = steeringAngleRad + angle3;
    int totalAngle = steeringAngle + angle3deg;
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
      x_dist = diag_dist3;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = -diag_dist3;
      y_dist = 0;
    } else {
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color3_x = camera_x + x_dist;
    color3_y = camera_y + y_dist;
    dx3 = hori_dist3;
    dy3 = sqrt(sq(diag_dist3)-sq(hori_dist3));
  } else {}
}
void getObstacleCoords4() {
  if (detected4 == 1){
    getCameraCoords();
    double x_dist, y_dist, x_dist_tmp, y_dist_tmp;
    double totalAngleRad = steeringAngleRad + angle4;
    int totalAngle = steeringAngle + angle4deg;
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
      x_dist = diag_dist4;
      y_dist = 0;
    } else if (totalAngle == -90) {
      x_dist = -diag_dist4;
      y_dist = 0;
    } else {
      x_dist = x_dist_tmp;
      y_dist = y_dist_tmp;
    }
    color4_x = camera_x + x_dist;
    color4_y = camera_y + y_dist;
    dx4 = hori_dist4;
    dy4 = sqrt(sq(diag_dist4)-sq(hori_dist4));
  } else {}
}

// for debugging, print obstacle coordinates to serial monitor
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
  delay(5000);
}
/*==================== END OF CALCULATION OF OBSTACLE COORDINATES ====================*/