// variables used for Serial Communication
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
char recvFromControl[MSG_BUFFER_SIZE];
char tmpFromControl[MSG_BUFFER_SIZE];
boolean newData = false;

// variables received from CONTROL from COMMAND
char tmpMode[2];        // A for Auto, M for Manual, O for Obstacle Coordinates. 
int driveMode;          // 0 for Automatic, 1 for Manual. for DRIVE's usage
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
int rover_x = 200;
int rover_y = 300;
int steeringAngle = 45;

// obstacle coordinates received from CONTROL
int color0_x, color1_x, color2_x, color3_x, color4_x;
int color0_y, color1_y, color2_y, color3_y, color4_y;
int dx0, dx1, dx2, dx3, dx4;   
int dy0, dy1, dy2, dy3, dy4;   


void setup() {
  Serial.begin(38400);
  Serial1.begin(9600);
}

void loop() {
  getControlData();
  sendToControl();
}

// Send rover data to CONTROL every 3 seconds
void sendToControl() {
  snprintf (msg, MSG_BUFFER_SIZE, "<%i,%i,%i>", rover_x, rover_y, steeringAngle);
  Serial1.write(msg);
  Serial.print("To CONTROL: rover coordinates and steering angle: "); // for debugging
  Serial.println(msg); // for debugging
  delay(3000);
}

void getControlData() {
  recvFromSerial1();
  if (newData == true) {
    Serial.println("From CONTROL: Instruction sent over by COMMAND"); // for debugging
    strcpy(tmpFromControl, recvFromControl); // for debugging
    parseControlData();
    printControlData();   // for debugging
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
  int tmpColor;
  strtokIndx = strtok(recvFromControl, ",");
  strcpy(tmpMode, strtokIndx);
  // automatic mode
  if (strcmp(tmpMode, "A") == 0) {
    strtokIndx = strtok(NULL, ",");
    target_x = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    target_y = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    radius = atoi(strtokIndx);
    driveMode = 0;
  }
  // manual mode
  if (strcmp(tmpMode, "M") == 0) {
    strtokIndx = strtok(NULL, ",");
    cmd_direction = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_dist = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_angle = atoi(strtokIndx);
    strtokIndx = strtok(NULL, ",");
    cmd_speed = atoi(strtokIndx);
    driveMode = 1;
  }
  // receiving obstacle coordinates from CONTROL
  if (strcmp(tmpMode, "O") == 0) {
    strtokIndx = strtok(NULL, ",");
    tmpColor = atoi(strtokIndx);
    switch (tmpColor) {
      case 0:
        strtokIndx = strtok(NULL, ",");
        color0_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color0_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx0 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy0 = atoi(strtokIndx);
        break;
      case 1:
        strtokIndx = strtok(NULL, ",");
        color1_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color1_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx1 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy1 = atoi(strtokIndx);
        break;
      case 2:
        strtokIndx = strtok(NULL, ",");
        color2_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color2_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx2 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy2 = atoi(strtokIndx);
        break;
      case 3:
        strtokIndx = strtok(NULL, ",");
        color3_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color3_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx3 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy3 = atoi(strtokIndx);
        break;
      case 4:
        strtokIndx = strtok(NULL, ",");
        color4_x = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        color4_y = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dx4 = atoi(strtokIndx);
        strtokIndx = strtok(NULL, ",");
        dy4 = atoi(strtokIndx);
        break;
    }
  }
}

// for debugging, print data received from CONTROL to serial monitor
void printControlData() {
  Serial.println("The following information is printed by printControlData function: ");
  if (strcmp(tmpMode, "A") == 0) {
    Serial.println("Automatic mode selected.");
    Serial.print("driveMode: ");
    Serial.println(driveMode);
    Serial.print("target_x: ");
    Serial.println(target_x);
    Serial.print("target_y: ");
    Serial.println(target_y);
    Serial.print("radius: ");
    Serial.println(radius);
    delay(3000);
  }
  if (strcmp(tmpMode, "M") == 0) {
    Serial.println("Manual mode selected.");
    Serial.print("driveMode: ");
    Serial.println(driveMode);
    Serial.print("direction (0 for forwards, 1 for backwards): ");
    Serial.println(cmd_direction);
    Serial.print("distance to travel: ");
    Serial.println(cmd_dist);
    Serial.print("desired rover angle: ");
    Serial.println(cmd_angle);
    Serial.print("desired rover speed: ");
    Serial.println(cmd_speed);
    delay(3000);
  }
  if (strcmp(tmpMode, "O") == 0) {
    Serial.println("Obstacle coordinates received. ");
    Serial.print("color0 x: ");
    Serial.print(color0_x);
    Serial.print(", color0 y: ");
    Serial.print(color0_y);
    Serial.print(", dx0: ");
    Serial.print(dx0);
    Serial.print(", dy0: ");
    Serial.println(dy0);
    Serial.print("color1 x: ");
    Serial.print(color1_x);
    Serial.print(", color1 y: ");
    Serial.print(color1_y);
    Serial.print(", dx1: ");
    Serial.print(dx1);
    Serial.print(", dy1: ");
    Serial.println(dy1);
    Serial.print("color2 x: ");
    Serial.print(color2_x);
    Serial.print(", color2 y: ");
    Serial.print(color2_y);
    Serial.print(", dx2: ");
    Serial.print(dx2);
    Serial.print(", dy2: ");
    Serial.println(dy2);
    Serial.print("color3 x: ");
    Serial.print(color3_x);
    Serial.print(", color3 y: ");
    Serial.print(color3_y);
    Serial.print(", dx3: ");
    Serial.print(dx3);
    Serial.print(", dy3: ");
    Serial.println(dy3);
    Serial.print("color4 x: ");
    Serial.print(color4_x);
    Serial.print(", color4 y: ");
    Serial.print(color4_y);
    Serial.print(", dx4: ");
    Serial.print(dx4);
    Serial.print(", dy4: ");
    Serial.println(dy4);
    delay(3000);
  }
}
