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

// variables received from COMMAND to send to DRIVE
// automatic mode
int target_x;       // target x-coordinate to travel to
int target_y;       // target y-coordinate to travel to
int radius;         // radius to sweep in automatic mode
// manual mode: can only be used after all obstacles have been detected
char cmd_direction[2];  // backwards or forwards
int cmd_dist;           // travel distance in terms of mm
int cmd_angle;          // left or right
int cmd_speed;          // rover speed

// variables received from VISION
int detected = 0;                       // 1 only if obstacle is detected, 0 other wise


// variables to send to COMMAND for mapping, and also send obstacle coords to DRIVE
int obstacle_x = 130;
int obstacle_y = 250;

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

  // publish obstacle coordinates to COMMAND through topic obstacleCoords every 3 sec, only if detected is HIGH
  currentTime = millis();
  if (currentTime - lastMsgTime > 5000) {
    lastMsgTime = currentTime;
    sendAllObstacleCoords();
  }
}

// =======================================================================

// FUNCTION DEFINITIONS

/*========================= COMMAND =========================*/
// function to send coordinates of obstacle to COMMAND
void sendAllObstacleCoords(){
  sendObstacleCoords(color0_x, color0_y, 0, detected0);
  sendObstacleCoords(color1_x, color1_y, 1, detected1);
  sendObstacleCoords(color2_x, color2_y, 2, detected2);
  sendObstacleCoords(color3_x, color3_y, 3, detected3);
  sendObstacleCoords(color4_x, color4_y, 4, detected4);
}

// function to send coordinates of obstacle to COMMAND
void sendObstacleCoords(int obstacle_x, int obstacle_y, int color, int detected) {
  if (detected == 1){
    snprintf (msg, MSG_BUFFER_SIZE, "%ld,%ld,%ld", obstacle_x, obstacle_y, color);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("obstacleCoords", msg);
  } else {}
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
    cmd_dist = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_angle = atoi(strtokIndx);
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
  Serial.print("cmd_dist: ");
  Serial.println(cmd_dist);
  Serial.print("cmd_angle: ");
  Serial.println(cmd_angle);
  Serial.print("cmd_speed: ");
  Serial.println(cmd_speed);
  delay(1000);
}
/*========================= END OF COMMAND =========================*/

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
