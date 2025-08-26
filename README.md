# Sharley Humanoid
### Introduction
Everything that is automated have its mind, we call it Motherboard. This is the code of Sharley's Motherboards which uses Wi-Fi and Web Server to intract with the controller or the other motherboard.
Sharley uses 2 Servo Motor Operating Motherboards for its arms and **this repository contains the code of those 2 motherboards only**.
For each motherboard, we used different motherboards to control all the 8 Servo Motors + 1 Head Servo Motor.

### Ports
QtPi Components used RJ Cables to connect the components to the motherboards, even though the pins are not mentioned on the motherboard, we figured them out.

**Motherboard 1 (Master Motherboard)**
 - PIN 32 (PORT 1): Right Arm's Shoulder's Right Servo Motor
 - PIN 33 (PORT 2): Right Arm's Shoulder's Left Servo Motor
 - PIN 23 (PORT 3): Right Arm's Elbow's Right Servo Motor
 - PIN 12 (PORT 4): Right Arm's Elbow's Left Servo Motor
 - PIN 27 (PORT 5): Head (90 degree default center)

**Motherboard 2 (MB2 || Slave Motherboard)**
 - PIN 32 (PORT 1): Left Arm's Shoulder's Right Servo Motor
 - PIN 33 (PORT 2): Left Arm's Shoulder's Left Servo Motor
 - PIN 23 (PORT 3): Left Arm's Elbow's Right Servo Motor
 - PIN 12 (PORT 4): Left Arm's Elbow's Left Servo Motor
