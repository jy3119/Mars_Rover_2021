# Mars Rover System - Group 25

This project implements a Mars Rover system consisting of 6 subsystems - Integration, Energy, Drive, Vision, Command and Control. The Mars Rover is capable of autonomously traversing and mapping out a remote location through remote control without the need for direct supervision. It is also able to detect, avoid, and map out obstacles encountered. Additionally, self charging capability was implemented, though not physically.

## Structural Design
The following diagram illustrates the Structural Design of the Mars rover system.
<br />

<img src="images/structural_design.png" />

## Functional Design
The following diagram illustrates the Functional Design of the Mars rover system.
<br />

<img src="images/functional_design.png" />

## Physical Implementation
The following diagram shows the physical implementation of the actual Mars Rover system.
<br />

<img src="images/physical_imple.png" />

# How to Use the Mars Rover system / Integration:
1. Plug in the ESP32 onto the FPGA.
2. Connect the ESP32 to the Arduino Nano Every on the Drive subsystem as following in the diagram below.
<br />

<img src="images/wiring_ESP32_and_Nano_Every.jpg" />

3. Ensure that all the different devices are connected to your computer via the appropriate cables for programminng.
4. To set up the Command subsystem, follow the instructions as seen in the _Command_ folder.
5. To set up the Control subsystem, follow the instructions as seen in the _Control_ folder.
6. To set up the Vision subsystem, follow the instructions as seen in the _Vision_ folder.
7. To set up the Drive subsystem, follow the instructions as seen in the _Drive_ folder. Take note to remove the SMPS cable before inserting the Arduino Nano Every to prevent short circuit from occuring due to pin misalignment.
8. The Energy subsystem cannot be physically implemented, please refer to the _Energy_ folder for further details on how it is integrated with the Mars Rover system.
9. The Mars Rover system is now capable of taking in movement commands from the Command subsystem, and capable of obstacle detection and avoidance, and terrain mapping.

## Folder Directory
Each subsystem has their own folders containing the instructions on how to set up and operate the respective subsystems. Both the final and testing files for each subsystem have also been included in each folder.

## Further Details
For further details and explanation, please refer to the Report and Video.

# Team Members
Melody Leom

Tser Ying Siew

Jeongin Yang

Xin Sun

Ziheng Qi

Zhengyangguang Bao



