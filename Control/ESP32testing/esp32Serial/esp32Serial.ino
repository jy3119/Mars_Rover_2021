#define TXD2 17 // look for port 8 on the arduino adaptor of the ESP32, connect this to RX on the arduino
#define RXD2 16 // look for port 9 on the arduino adaptor of the ESP32, connect this to TX on the arduino

// variables used for Serial Communication
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
char recvFromDrive[MSG_BUFFER_SIZE];
char tmpFromDrive[MSG_BUFFER_SIZE];
boolean newData = false;

// variables received from DRIVE
int rover_x;
int rover_y;
int steeringAngle;

// variables received from COMMAND to send to DRIVE
int driveMode=0;
// automatic mode
int target_x=200;           // target x-coordinate to travel to
int target_y=300;           // target y-coordinate to travel to
int radius=50;             // radius to sweep in automatic mode
// manual mode: can only be used after all obstacles have been detected
int cmd_direction=0;      // 0 for forwards, 1 for backwards
int cmd_dist=500;           // travel distance in terms of mm
int cmd_angle=45;          // -180 to 180, ACW is positive, CW is negative
int cmd_speed=5;          // rover speed


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
 getDriveData();
 sendToDrive();
}

// Send Data to DRIVE every 10 seconds
void sendToDrive() {
  // automatic mode
  if (driveMode == 0) {
    snprintf (msg, MSG_BUFFER_SIZE, "<%c,%i,%i,%i,%i>", 'A', target_x, target_y, radius);
  }
  // manual mode
  if (driveMode == 1) {
    snprintf (msg, MSG_BUFFER_SIZE, "<%c,%i,%i,%i,%i,%i>", 'M', cmd_direction, cmd_dist, cmd_angle, cmd_speed);
  }
  Serial2.write(msg);
  delay(10000);
}

void getDriveData() {
  recvFromSerial2();
  if (newData == true) {
    Serial.println(recvFromDrive);
    strcpy(tmpFromDrive, recvFromDrive);
    parseDriveData();
    printDriveData();
    newData = false;
  }
}

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

void parseDriveData() {
  char * strtokIndx;
  strtokIndx = strtok(recvFromDrive, ",");
  rover_x = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  rover_y = atoi(strtokIndx);
  strtokIndx = strtok(NULL, ",");
  steeringAngle = atoi(strtokIndx);
}

void printDriveData() {
  Serial.print("rover_x: ");
  Serial.println(rover_x);
  Serial.print("rover_y: ");
  Serial.println(rover_y);
  Serial.print("steeringAngle: ");
  Serial.println(steeringAngle);
  delay(5000);
}
