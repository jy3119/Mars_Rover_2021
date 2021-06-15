Code for Arduino nano every contains of two parts Optical Sensor and Motor Control
optical sensor: code for operating adns_3080, calculating steering direction and estimating exact coordinate
motor control: code for six different modes of motion, move by distance or angle and more complex function like polar coordinate control ,obstacle detection and avoidance.
but the motor control file cannot run seperately since many function require the position information and the input from other subsystems.
combination_finalv is the final version that combines all the function of Drive module. 