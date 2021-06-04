 #include <math.h>

// variables received from DRIVE
int rover_x = 0;            // current x-coordinates of rover
int rover_y = 0;            // current y-coordinates of rover
int steeringAngle = 180;     // steering angle of rover, ACW +ve, CW -ve, range is -180 to 180 degrees

// variables received from VISION
int angle = 0;                          // angle of osbtacle to rover. on left of rover = +ve, on right of rover = -ve
int diag_dist = 500;                     // diagonal distance of obstacle to rover

// variables for calculation of obstacle coords
int camera_x, camera_y;      // x and y coordinates of rover's front camera corrected for rover length
int roverLength = 1000;      // NEED INTEGRATION/DRIVE'S INPUT: distance between coordinate detector and camera of rover, i'm assuming its 1000mm/10cm in this case
int steerQuadrant, angleQuadrant;           

// variables to send to COMMAND
int obstacle_x; 
int obstacle_y;

void setup() {
  Serial.begin(115200);
}

void loop() {
  getCameraCoords();
  if (Serial.available() > 0){
    angle = Serial.readString().toInt();
  }
  getObstacleCoords();
}

void getSteerQuadrant(){
  if (steeringAngle == 0){
    steerQuadrant = 1;
  } else if ((steeringAngle > 0) && (steeringAngle < 90)) {
    steerQuadrant = 2;
  } else if (steeringAngle == 90){
    steerQuadrant = 3;
  } else if ((steeringAngle > 90) && (steeringAngle < 180)) {
    steerQuadrant = 4;
  } else if ((steeringAngle == 180) || (steeringAngle == -180)){
    steerQuadrant = 5;
  } else if ((steeringAngle < 0) && (steeringAngle > -90)) {
    steerQuadrant = 6;
  } else if (steeringAngle == -90){
    steerQuadrant = 7;
  } else if ((steeringAngle < -90) && (steeringAngle > -180)) {
    steerQuadrant = 8;
  }
}

void getAngleQuadrant(){
  if (angle == 0){
    angleQuadrant = 0;
  } else if ((angle > 0) && (angle < 90)) {
    angleQuadrant = 1;
  } else if ((angle < 0) && (angle > -90)) {
    angleQuadrant = 2;
  }
  Serial.print("obstacle angle: ");
  Serial.println(angle);
  Serial.print("angle quadrant: ");
  Serial.println(angleQuadrant);
}

// account for distance between camera of rover and obstacle detection device on rover
void getCameraCoords(){
  float x_dist_tmp, y_dist_tmp, x_dist, y_dist;
  float steeringAngleRad = (steeringAngle * 71) / 4068.0;
  x_dist_tmp = roverLength * sin(steeringAngleRad);
  y_dist_tmp = roverLength * cos(steeringAngleRad);
  getSteerQuadrant();
  switch (steerQuadrant) {
    case 1: // steering angle = 0, i.e. rover is facing straight ahead
      x_dist = 0;
      y_dist = roverLength;
      break;
    case 2: // steering angle between 0 and 90 degrees: x -ve, y +ve
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 3: // steering angle = 90, i.e. rover is facing 90 degrees to the left / ACW
      x_dist = -roverLength;
      y_dist = 0;
      break;
    case 4: // steering angle between 90 and 180 degrees: x -ve, y -ve
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 5: // steering angle = 180 or -180, i.e. rover is facing straight behind
      x_dist = 0;
      y_dist = -roverLength;
      break;
    case 6: // steering angle between 0 and -90 degrees: x +ve, y +ve
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
    case 7: // steering angle = -90, i.e. rover is facing 90 degrees to the right / CW
      x_dist = roverLength;
      y_dist = 0;
      break;
    case 8: // steering angle between -90 and -180 degrees: x +ve, y -ve
      x_dist = -x_dist_tmp;
      y_dist = y_dist_tmp;
      break;
  }
  camera_x = rover_x + x_dist;
  camera_y = rover_y + y_dist;
  // printCameraCoords(); // print to serial monitor for debugging purposes
}

void printCameraCoords(){
  Serial.print("x-coordinates of camera: ");
  Serial.println(camera_x);
  Serial.print("y-coordinates of camera: ");
  Serial.println(camera_y);
  delay(3000);
}

void getObstacleCoords(){
  float x_dist, y_dist, x_dist_tmp, y_dist_tmp;
  int totalAngle_tmp = steeringAngle + angle;
  if (totalAngle_tmp )
  float angleRad = (angle * 71) / 4068.0;
  float totalAngleRad = (totalAngle * 71) / 4068.0;
  x_dist_tmp = roverLength * sin(totalAngleRad);
  y_dist_tmp = roverLength * cos(totalAngleRad);
  getSteerQuadrant();
  getAngleQuadrant();
  
  switch(steerQuadrant){
    case 1: // steering angle = 0, i.e. rover facing frontwards
      switch(angleQuadrant){
        case 0: // similar to steerQuadrant case 1
          x_dist = 0;
          y_dist = diag_dist;
          break;
        case 1: // similar to steerQuadrant case 2
          x_dist = -x_dist_tmp;
          y_dist = y_dist_tmp;
          break;
        case 2: // similar to steerQuadrant case 6
          x_dist = -x_dist_tmp;
          y_dist = y_dist_tmp;
          break;
      }
      break;  
    case 5: // steering angle = 180 or - 180, i.e. rover facing backwards
      switch(angleQuadrant){
        case 0: // similar to steerQuadrant case 5
          x_dist = 0;
          y_dist = -diag_dist;
          break;
        case 1: // similar to steerQuadrant case 8
          x_dist = -x_dist_tmp;
          y_dist = y_dist_tmp;
          break;
        case 2: // similar to steerQuadrant case 4
          x_dist = -x_dist_tmp;
          y_dist = y_dist_tmp;
          break;
      }
      break;
    
  }
  
  obstacle_x = camera_x + x_dist;
  obstacle_y = camera_y + y_dist;
  printObstacleCoords();
}

void printObstacleCoords(){
  Serial.print("x-coordinates of obstacle: ");
  Serial.println(obstacle_x);
  Serial.print("y-coordinates of obstacle: ");
  Serial.println(obstacle_y);
  delay(3000);
}
