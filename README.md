<img width="1500" height="750" alt="2" src="https://github.com/user-attachments/assets/eb89fca8-eb20-41a7-a977-acf259184378" />

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

### Nicknames
To make the whole documentation, we have used nicknames as described below:

We used 3 words to describe the servo motors. 
First word is can be L (Left), R (Right) or H (Head), if the letter is L or R, it denotes that it is being used in Arms, else, it is being used as Head Servo.
Second word is either S (Shoulder) or E (Elbow) which tells the group of the servo motor i.e. Shoulder and Elbow.
Third and Final word is either L (Left) or R (Right) which tells the position of the servo motor i.e. Left or Right.

Here's list of all the servo nicknames:

- Right Arm's Shoulder's Right Servo Motor: RSR
- Right Arm's Shoulder's Left Servo Motor: RSL
- Right Arm's Elbow's Right Servo Motor: RER
- Right Arm's Elbow's Left Servo Motor: REL
- Left Arm's Shoulder's Right Servo Motor: LSR
- Left Arm's Shoulder's Left Servo Motor: LSL
- Left Arm's Elbow's Right Servo Motor: LER
- Left Arm's Elbow's Left Servo Motor: LEL
- Head: H

### Angles
The below given angles are the angles of rotation of Servo Motors:

**DEFAULT/CLOSE ARMS**
- RSR, RSL, RER, REL, LSR, LSL, LER, LEL: 90 degrees

**OPEN ARMS**
- RSL, REL, LSL, LEL: 180 degrees
- RSR, RSR, LSR, RER: 0 degrees

**HEAD**
- CENTER: 90 degrees
- LEFT: 30 degrees
- RIGHT: 150 degrees

### SETUP

CLONE/DOWNLOAD this source code and follow these steps to setup your motherboard!

**STEP 1: PORTS SETUP**
Ports are listed above along with servo motors, DO check them and setup your physical part according to them, any mistake can cause errors in real working!

**STEP 2: SETTING UP ARDUINO IDE (IF YOU ARE USING QtPi's Veda V3.0 ESP32)**
- Click on `File` tab on tab bar.
- Choose `Preferences` option.
- In **Additional boards manager URLs**, paste this: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`.
- Click `OK`

**STEP 3: SETTING UP SLAVE**
- Open SlaveCode Folder and look for `SlaveCode.ino` file in it.
- Open `SlaveCode.ino` file.
- Look for `SSID` CONST and enter your WiFi's SSID and `Password` CONST and enter your WiFi's Password.
- Upload the code to Motherboard 2 **(NOTE: UPLOADING CODE WILL ERASE ALL THE CODE IN THE MOTHERBOARD WHICH WILL RESULT IN NO-CONNECTION WITH QtPi OR CODE2PLAY)**
- You will obtain a IP Address (If connection successful with WiFi) in Serial Monitor on `115200`, Copy that IP, DON'T POWER OFF THE MOTHERBOARD.

**STEP 4: SETTING UP MASTER**
- Open MasterCode folder and look for `MasterCode.ino` file in it.
- Open `MasterCode.ino` file.
- Look for `SSID` CONST and enter your WiFi's SSID as its value and look for `Password` CONST and enter your WiFi's Password as its value.
- Look for `slaveIP` CONST and enter your Motherboard's IP that we optained in Step 3 as its value.
- Upload the code to Motherboard 1 **(NOTE: UPLOADING CODE WILL ERASE ALL THE CODE IN THE MOTHERBOARD WHICH WILL RESULT IN NO-CONNECTION WITH QtPi OR CODE2PLAY)**
- You will obtain a IP Address (If connection successfull with WiFi) in Serial Monitor on `115200`, copy that IP, DON'T POWER OFF BOTH MOTHERBOARDS.

### RUNNING & TESTING

- Open browser on any device connected to the same network as Motherboards.
- Put the Master Motherboard's IP in the address bar.
- You will see some buttons i.e. `OPEN`, `CLOSE`, `CENTER`, `LEFT`, `RIGHT`


  YOU ARE GOOD TO GO!
  ENJOY YOUR OWN SHARLEY, GO INTO THE CODE FOR CUSTOMIZATIONS!
