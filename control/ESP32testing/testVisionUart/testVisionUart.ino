#define RXD1 21
#define TXD1 22

int color;
int detected0, detected1, detected2, detected3, detected4;              // 1 only if obstacle is detected, 0 other wise
int diag_dist0, diag_dist1, diag_dist2, diag_dist3, diag_dist4;         // diagonal distance of obstacle to rover, received in cm
int hori_dist0, hori_dist1, hori_dist2, hori_dist3, hori_dist4;         // horizontal distance of obstacle to rover, received in mm
double angle0, angle1, angle2, angle3, angle4;                          // angle of osbtacle to rover. on left of rover = +ve,  on right of rover = -ve.  range: -90 to 90 degrees
double angle0deg, angle1deg, angle2deg, angle3deg, angle4deg;

#define MSG_BUFFER_SIZE (50)
char recvFromVision[MSG_BUFFER_SIZE];
char tmpFromVision[MSG_BUFFER_SIZE];
boolean newDataV = false;

void setup() {
  Serial.begin(115200);
  setupVision();
}

void loop() {
  getVisionData();
}


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
      double ratio = (double)hori_dist0 / (double)diag_dist0;
      angle0 = asin(ratio); 
      angle0deg = (angle0 * 4068) / 71.0;
      break;
    case 1:
      strtokIndx = strtok(NULL, ",");
      detected1 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist1 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist1 = atoi(strtokIndx);
      double ratio = (double)hori_dist1 / (double)diag_dist1;
      angle1 = asin(ratio); 
      angle1deg = (angle1 * 4068) / 71.0;
      break;
    case 2:
      strtokIndx = strtok(NULL, ",");
      detected2 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist2 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist2 = atoi(strtokIndx);
      double ratio = (double)hori_dist2 / (double)diag_dist2;
      angle2 = asin(ratio); 
      angle2deg = (angle2 * 4068) / 71.0;
      break;
    case 3:
      strtokIndx = strtok(NULL, ",");
      detected3 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist3 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist3 = atoi(strtokIndx);
      double ratio = (double)hori_dist3 / (double)diag_dist3;
      angle3 = asin(ratio); 
      angle3deg = (angle3 * 4068) / 71.0;
      break;
    case 4:
      strtokIndx = strtok(NULL, ",");
      detected4 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      hori_dist4 = atoi(strtokIndx);
      strtokIndx = strtok(NULL, ",");
      diag_dist4 = atoi(strtokIndx);
      double ratio = (double)hori_dist4 / (double)diag_dist4;
      angle4 = asin(ratio); 
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
      Serial.print(", angle0 (rad): ");
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
      Serial.print(", angle1 (rad): ");
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
      Serial.print(", angle2 (rad): ");
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
      Serial.print(", angle3 (rad): ");
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
      Serial.print(", angle4 (rad): ");
      Serial.print(angle4);
      Serial.print(", angle4 (deg): ");
      Serial.println(angle4deg);
      break;
  }
}