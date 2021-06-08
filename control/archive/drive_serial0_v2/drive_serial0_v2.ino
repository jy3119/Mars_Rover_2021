// edit the pin numbers according to arduino nano every's pin assignment


// variables used for Serial Communication
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
char recvFromControl[MSG_BUFFER_SIZE];
char tmpFromControl[MSG_BUFFER_SIZE];
boolean newData = false;

// variables received from CONTROL from COMMAND
int driveMode;
// automatic mode
int target_x;           // target x-coordinate to travel to
int target_y;           // target y-coordinate to travel to
int radius;             // radius to sweep in automatic mode
// manual mode: can only be used after all obstacles have been detected
int cmd_direction;      // 0 for forwards, 1 for backwards
int cmd_dist;           // travel distance in terms of mm
int cmd_angle;          // -180 to 180, ACW is positive, CW is negative
int cmd_speed;          // rover speed


// variables to send to CONTROL, these values will actually change as the rover moves on the full Drive code
int rover_x=200;
int rover_y=300;
int steeringAngle=45;


void setup(){
  Serial.begin(38400);
  Serial1.begin(9600);
}

void loop(){
 getControlData();   
 sendToControl();
}

// Send data to CONTROL every second
void sendToControl(){
 snprintf (msg, MSG_BUFFER_SIZE, "<%i,%i,%i>", rover_x, rover_y, steeringAngle);
 Serial1.write(msg);
 delay(1000);
}

void getControlData(){
  recvFromSerial1();
  if (newData == true) {
    strcpy(tmpFromControl, recvFromControl);
    parseControlData();
    printControlData();
    newData = false;
  }
}

void recvFromSerial1() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        recvFromControl[ndx] = rc;
        ndx++;
        if (ndx >= MSG_BUFFER_SIZE) {
          ndx = MSG_BUFFER_SIZE - 1;
        }
      }
      else {
        recvFromControl[ndx] = '\0'; // terminate the string
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

void parseControlData() {
  char * strtokIndx;
  strtokIndx = strtok(recvFromControl, ",");
  driveMode = atoi(strtokIndx);
  if (driveMode == 1) {
    strtokIndx = strtok(NULL, ",");
    cmd_direction = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_dist = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_angle = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_speed = atoi(strtokIndx);
  } else {
    strtokIndx = strtok(NULL, ",");
    target_x = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_y = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    radius = atoi(strtokIndx);
  }
}

void printControlData() {
  if (driveMode == 0){
    Serial.println("Automatic mode selected.");
    Serial.print("target_x: ");
    Serial.println(target_x);
    Serial.print("target_y: ");
    Serial.println(target_y);
    Serial.print("radius: ");
    Serial.println(radius);
  } else if (driveMode == 1){
    Serial.println("Manual mode selected.");
    Serial.print("direction (0 for forwards, 1 for backwards): ");
    Serial.println(cmd_direction);
    Serial.print("distance to travel: ");
    Serial.println(cmd_dist);
    Serial.print("desired rover angle: ");
    Serial.println(cmd_angle);
    Serial.print("desired rover speed: ");
    Serial.println(cmd_speed);
  }
}
