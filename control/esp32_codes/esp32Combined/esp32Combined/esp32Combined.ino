#include <WiFi.h>
#include <PubSubClient.h>
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

// variables received from COMMAND to send to DRIVE
int target_x;
int target_y;
int target_dist;
int target_angle;
int target_speed;
int radius_dist;

// variables received from VISION
long BASE_ADDRESS = 0x40000;        // check platform designer in Quartus for BASE_ADDRESS of I2C_MEM
int8_t byte0, byte1, byte2, byte3;
int detected;                       // 1 only if obstacle is detected, 0 other wise
int angle;                          // angle of osbtacle to rover. on left of rover = +ve, on right of rover = -ve
int diag_dist;                      // diagonal distance of obstacle to rover


// variables to send to COMMAND
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

  getVisionDataI2C();    // receive data from VISION via I2C

  // publish obstacle coordinates to COMMAND through topic obstacleCoords every 3 sec, only if detected_0 is 1
    currentTime = millis();
    if (detected == 1 && currentTime - lastMsgTime > 3000) {
      lastMsgTime = currentTime;
      sendObstacleCoords();
    }
}

// =======================================================================

// FUNCTION DEFINITIONS

// function to send coordinates of obstacle to COMMAND
void sendObstacleCoords() {
  snprintf (msg, MSG_BUFFER_SIZE, "%ld,%ld", obstacle_x, obstacle_y);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("obstacle_coords", msg, true);
}

// for debugging, function to print data received from COMMAND
void printCommandData() {
  Serial.print("target_x: ");
  Serial.println(target_x);
  Serial.print("target_y: ");
  Serial.println(target_y);
  Serial.print("target_dist: ");
  Serial.println(target_dist);
  Serial.print("target_angle: ");
  Serial.println(target_angle);
  Serial.print("target_speed: ");
  Serial.println(target_speed);
  Serial.print("radius_dist: ");
  Serial.println(radius_dist);
  delay(1000);
}

// parse data received from COMMAND and store into variables as needed
void parseData(char* topic, char incomingData[numChars]) {
  if (strcmp(topic, "coordsMode") == 0) {
    char * strtokIndx;
    strtokIndx = strtok(incomingData, ",");
    target_x = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_y = atoi(strtokIndx);
  }
  if (strcmp(topic, "instructionsMode") == 0) {
    char * strtokIndx;
    strtokIndx = strtok(incomingData, ",");
    target_dist = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_angle = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_speed = atoi(strtokIndx);
  }
  if (strcmp(topic, "radiusMode") == 0) {
    radius_dist = atoi(incomingData);
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
      client.subscribe("coordsMode");
      client.subscribe("instructionsMode");
      client.subscribe("radiusMode");

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

// receiving data from FPGA via I2C protocol
void getVisionDataI2C() {
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
      diag_dist = ((byte3<<8) + byte2) + 1;
     } else {
      diag_dist = ((byte3<<8) + byte2);
    }
    printVisionData(); // print variables received from VISION to serial monitor, for debugging 
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
