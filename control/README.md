# Control Subsystem

Files for Control subsystem.

# How to use:
1. Download the Arduino IDE from here: https://www.arduino.cc/en/software
2. Download the ESP32 library by following the instructions here: 
   https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md
3. Download the PubSubClient library from https://github.com/knolleary/pubsubclient and add it into your Arduino IDE.
4. Download the ControlLib3 folder and add this library into your Arduino IDE.
5. Download the ControlESP32.ino file
6. Edit the name and password of your WiFi network in the first few lines of the ControlESP32.ino file.
7. Upload the code onto your ESP32

## Folder Directory:
```bash
control
├── ControlESP32
│   └── ControlESP32.ino                // FOR UPLOAD TO ESP32
├── ControlLib3                         // FOR DOWNLOAD, library containing all functions and variables needed for CONTROL to work
│   ├── ControlLib3.cpp
│   └── ControlLib3.h
├── ESP32testing                        // ESP32 TEST .ino FILES
│   ├── esp32FULL
│   │   └── esp32FULL.ino                 // full functionality ESP32 code
│   ├── calculateObstCoords
│   │   └── calculateObstCoords.ino       // test calculation of obstacle coordinates
│   ├── esp32Serial
│   │   └── esp32Serial.ino               // test Serial/UART communication with DRIVE
│   ├── i2c_vision_fpga
│   │   └── i2c_vision_fpga.ino           // test I2C connection with VISION
│   ├── testVisionUart
│   │   └── testVisionUart.ino            // test UART communication with VISION
│   └── testMQTT
│       └── testMQTT.ino                  // test MQTT connection with COMMAND
├── drive_serial_v3
│   └── drive_serial_v3.ino             // .ino file containing code for Serial/UART commmunication for DRIVE's Arduino Nano Every
├── vision_fpga                         // quartus/eclipse files for VISION's FPGA for UART and I2C communication (test version, not integrated with camera)
│   ├── i2c_vision_fpga
│   └── uart_vision
│       ├── esp32_uart.v
│       ├── nios2_cpu.qsys
│       └── uart_fpga_test.c
├── archive
└── README.md
 
```

