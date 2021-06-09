# Control Subsystem

Files for Control subsystem.

## Folder Directory:
```bash
control
├── ControlESP32
│   └── ControlESP32.ino                // FOR UPLOAD TO ESP32
├── ControlLib2                         // FOR DOWNLOAD, library containing all functions and variables needed for CONTROL to work
│   ├── ControlLib2.cpp
│   └── ControlLib2.h
├── ESP32testing                        // ESP32 TEST .ino FILES
│   ├── esp32FULL
│   │   └── esp32FULL.ino                 // full functionality ESP32 code
│   ├── calculateObstCoords
│   │   └── calculateObstCoords.ino       // test calculation of obstacle coordinates
│   ├── esp32Serial
│   │   └── esp32Serial.ino               // test Serial/UART communication with DRIVE
│   ├── i2c_vision_fpga
│   │   └── i2c_vision_fpga.ino           // test I2C connection with VISION
│   └── testMQTT
│       └── testMQTT.ino                  // test MQTT connection with COMMAND
├── drive_serial_v3
│   └── drive_serial_v3.ino             // .ino file containing code for Serial/UART commmunication for DRIVE's Arduino Nano Every
├── vision_fpga                         // quartus/eclipse files for VISION's FPGA for I2C communication (test version, not integrated with camera)
    ├── README.md
    ├── i2c_adaptor.v
    ├── i2c_fpga_test.c
    ├── nios2_cpu.qsys
    ├── pin_assignment.qsf
    └── top_level.v
├── archive
└── README.md

 
```

