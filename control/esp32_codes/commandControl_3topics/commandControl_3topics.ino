#include <WiFi.h>
#include <PubSubClient.h>

// UPDATE HERE ACCORDING TO YOUR WIFI Name and password, and MQTT Server's Public DNS
const char* ssid = "AH LAM";
const char* password = "96258167";
const char* mqtt_server = "ec2-3-21-76-51.us-east-2.compute.amazonaws.com";
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

// variables to send to DRIVE
int target_x;
int target_y;
int target_dist;
int target_angle;
int target_speed;
int radius_dist;

// variables to send to COMMAND
int obstacle_x = 200;
int obstacle_y = 300;
int detected_0 = 1;
int obst_x_last, obst_y_last;


// =======================================================================

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_mqtt();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  recvWithEndMarker();
  detected_0 = atoi(receivedChars);

  // publish obstacle coordinates to COMMAND through topic obstacle_coords every 2 sec, only if detected_0 is 1
    currentTime = millis();
    if (detected_0 == 1 && currentTime - lastMsgTime > 2000) {
      lastMsgTime = currentTime;
      sendObstacleCoords();
    }
}

// =======================================================================

// FUNCTION DEFINITIONS

void sendObstacleCoords() {
  snprintf (msg, MSG_BUFFER_SIZE, "%ld,%ld", obstacle_x, obstacle_y);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("obstacle_coords", msg, true);
}

void printDriveData() {
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
  delay(10000);
}

// parse data and store into variables as needed
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

// function to execute when a message is received for a subscribed topic
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
  printDriveData();
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
