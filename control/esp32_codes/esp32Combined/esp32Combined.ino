#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>
#include <Wire.h>

// for connecting ESP32 to wifi and MQTT server
// UPDATE HERE ACCORDING TO YOUR WIFI Name and password, and MQTT Server's Public DNS
const char* ssid = "AH LAM";
const char* password = "96258167";
const char* mqtt_server = "ec2-18-224-199-255.us-east-2.compute.amazonaws.com";
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

// automatic mode
// variables received from COMMAND to send to DRIVE
int target_x;       // target x-coordinate to travel to
int target_y;       // target y-coordinate to travel to
int radius;         // radius to sweep in automatic mode
// manual mode: can only be used after all obstacles have been detected
char cmd_direction[2];  // backwards or forwards
char cmd_angle[2];      // left or right
int cmd_speed;          // rover speed


// variables received from VISION
long BASE_ADDRESS = 0x80000;        // check platform designer in Quartus for BASE_ADDRESS of I2C_MEM
int8_t byte0, byte1, byte2, byte3;
int detected;                       // 1 only if obstacle is detected, 0 other wise
int angle;                          // angle of osbtacle to rover. on left of rover = +ve, on right of rover = -ve. range: -90 to 90 degrees
int diag_dist;                      // diagonal distance of obstacle to rover

// variables received from DRIVE
int rover_x;          // current x-coordinates of rover
int rover_y;          // current y-coordinates of rover
int steeringAngle = 0;    // steering angle of rover, ACW +ve, CW -ve, range is -180 to 180 degrees

// variables for calculation of obstacle coords
int camera_x, camera_y;          // x and y coordinates of rover's front camera corrected for rover length
int roverCorrection = 220;       // correction for distance between coordinate detector and camera of rover
int steerQuadrant;
int dx, dy;                      // send to DRIVE from CONTROL for automatic mode

// variables to send to COMMAND for mapping, and also send obstacle coords to DRIVE
int obstacle_x;
int obstacle_y;

int obst_x_last, obst_y_last;



// =======================================================================

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

  getVisionData();     // receive data from VISION via I2C
  getObstacleCoords();

  // publish obstacle coordinates to COMMAND through topic obstacleCoords every 3 sec, only if detected is HIGH
  currentTime = millis();
  if (detected == 1 && currentTime - lastMsgTime > 3000) {
    lastMsgTime = currentTime;
    sendObstacleCoords();
  }
}

// =======================================================================

// FUNCTION DEFINITIONS

/*========================= COMMAND =========================*/
// function to send coordinates of obstacle to COMMAND
void sendObstacleCoords() {
  snprintf (msg, MSG_BUFFER_SIZE, "%ld,%ld", obstacle_x, obstacle_y);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("obstacle_coords", msg, true);
}

// parse data received from COMMAND and store into variables as needed
void parseData(char* topic, char incomingData[numChars]) {
  if (strcmp(topic, "auto") == 0) {
    char * strtokIndx;
    strtokIndx = strtok(incomingData, ",");
    target_x = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_y = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    radius = atoi(strtokIndx);
  }
  if (strcmp(topic, "manual") == 0) {
    char * strtokIndx;
    strtokIndx = strtok(incomingData, ",");
    strcpy(cmd_direction, strtokIndx);
    strtokIndx = strtok(NULL, ",");
    strcpy(cmd_angle, strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_speed = atoi(strtokIndx);
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
  parseData(topic, incomingData);
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
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
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
  Serial.print("target_x: ");
  Serial.println(target_x);
  Serial.print("target_y: ");
  Serial.println(target_y);
  Serial.print("radius: ");
  Serial.println(radius);
  Serial.print("cmd_direction: ");
  Serial.println(cmd_direction);
  Serial.print("cmd_angle: ");
  Serial.println(cmd_angle);
  Serial.print("cmd_speed: ");
  Serial.println(cmd_speed);
  delay(1000);
}
/*========================= END OF COMMAND =========================*/

/*============================= VISION =============================*/
// receiving data from FPGA via I2C protocol
void getVisionData() {
  setAddress(BASE_ADDRESS);
  Wire.requestFrom(0x55, 4);    // request 1 word (4 bytes) from FPGA
  if (Wire.available() > 0) {
    // read incoming bytes from I2C
    byte0 = (Wire.read());
    byte1 = (Wire.read());
    byte2 = (Wire.read());
    byte3 = (Wire.read());

    // store values read from VISION into variables
    detected = byte0;
    angle = byte1;
    if (angle < 0) {
      diag_dist = (((byte3 << 8) + byte2) + 1) * 10;
    } else {
      diag_dist = ((byte3 << 8) + byte2) * 10;
    }
    printVisionData(); // print variables received from VISION to serial monitor, for debugging
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
/*========================= END OF VISION =========================*/

/*============== CALCULATION OF OBSTACLE COORDINATES ==============*/
// caculate x and y coordinates of obstacle
void getObstacleCoords() {
  getCameraCoords();
  float x_dist, y_dist, x_dist_tmp, y_dist_tmp;
  int totalAngle = steeringAngle + angle;
  float totalAngleRad = (totalAngle * 71) / 4068.0;
  x_dist_tmp = diag_dist * sin(totalAngleRad);
  y_dist_tmp = diag_dist * cos(totalAngleRad);
  getSteerQuadrant();
  if (totalAngle == 0) {
    x_dist = 0;
    y_dist = diag_dist;
  } else if ((totalAngle == 180) || (totalAngle == 180)) {
    x_dist = 0;
    y_dist = -diag_dist;
  } else if (totalAngle == 90) {
    x_dist = -diag_dist;
    y_dist = 0;
  } else if (totalAngle == -90) {
    x_dist = diag_dist;
    y_dist = 0;
  } else {
    x_dist = -x_dist_tmp;
    y_dist = y_dist_tmp;
  }
  obstacle_x = camera_x + x_dist;
  obstacle_y = camera_y + y_dist;
  dx = x_dist;
  dy = y_dist;
//  printObstacleCoords(); // print to serial monitor for debugging purposes
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
  Serial.print("x-coordinates of obstacle: ");
  Serial.println(obstacle_x);
  Serial.print("y-coordinates of obstacle: ");
  Serial.println(obstacle_y);
  delay(3000);
}
// print camera coordinates to serial monitor, for debugging
void printCameraCoords() {
  Serial.print("x-coordinates of camera: ");
  Serial.println(camera_x);
  Serial.print("y-coordinates of camera: ");
  Serial.println(camera_y);
  delay(3000);
}
/*========== END OF CALCULATION OF OBSTACLE COORDINATES ==========*/

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
