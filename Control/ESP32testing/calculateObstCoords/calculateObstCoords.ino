/*most updated algorithm found in ControlLib2*/
#include <math.h>

// variables received from DRIVE
int rover_x = 0;              // current x-coordinates of rover
int rover_y = 0;              // current y-coordinates of rover
int steeringAngle = 0;        // steering angle of rover, ACW +ve, CW -ve, range is -180 to 180 degrees

// variables received from VISION
int detected0=1, detected1=1, detected2=1, detected3=0, detected4=1;
int angle0=0, angle1=30, angle2=45, angle3=-30, angle4=-45;
int diag_dist0=50, diag_dist1=50, diag_dist2=50, diag_dist3=50, diag_dist4=50;

// variables for calculation of obstacle coords
int camera_x, camera_y;      // x and y coordinates of rover's front camera corrected for rover length
int roverCorrection = 220;       // distance between coordinate detector and camera of rover, i'm assuming its 1000mm/10cm in this case
int steerQuadrant;

// variables to send to COMMAND
int color0_x, color1_x, color2_x, color3_x, color4_x;
int color0_y, color1_y, color2_y, color3_y, color4_y;

// variables to send to DRIVE
int dx0, dx1, dx2, dx3, dx4;
int dy0, dy1, dy2, dy3, dy4;
void setup() {
  Serial.begin(115200);
}

void loop() {
  getAllObstacleCoords();
}

void getAllObstacleCoords(){
  getObstacleCoords0();
  getObstacleCoords1();
  getObstacleCoords2();
  getObstacleCoords3();
  getObstacleCoords4();
  printObstacleCoords();
}

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

// account for distance between camera of rover and obstacle detection device on rover
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
}

void printCameraCoords() {
  Serial.print("x-coordinates of camera: ");
  Serial.println(camera_x);
  Serial.print("y-coordinates of camera: ");
  Serial.println(camera_y);
  delay(3000);
}
