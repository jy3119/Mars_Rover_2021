# Drive Subsystem

Files for Drive subsystem.

# How to use:
The following steps are assuming that the Mars Rover system has already been fully assembled.
1. Go to https://www.arduino.cc/en/software and download the Arduino IDE 1.8.13
2. In the Arduino IDE, Open the boards manager
3. Search for and install the INA219WE library
4. Select the Arduino Nano Every as your board and select Registers Emulation: None
5. Connect your Arduino Nano Every board to computer and upload the final_drive_with_comms.ino file

Note that once the .ino file has been uploaded, it is not necessary to keep the Arduino Nano Every connected to your computer for the rover to function.
However, to observe the Serial Monitor for debugging purposes, it is necessary to keep the Arduino Nano Every connected to your computer.

# Folder Directory:
```bash
Drive
|
├── final_drive_with_comms.ino // FOR UPLOAD TO ARDUINO NANO EVERY
|
├── Optical_Sensor.ino     // code for operating adns_3080, calculating steering direction and     estimating exact coordinate
|
│                  
├── Motor_Control.ino      // code for six different modes of motion, move by distance or angle and more complex function like polar coordinate control ,obstacle detection and avoidance.
but the motor control file cannot run seperately since many function require the position information and the input from other subsystems.

│   
├── combination_finalv.ino // before testing with Integration

└── README.md
```
