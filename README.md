# OttoMower_ReP_AL

## ReP_AL
This is a customization of project: [ReP_AL-3D-Lawn-Mower](https://github.com/REPALphilread/ReP_AL-3D-Lawn-Mower)

This projec is based on the: Mower_V6.31

**!! All my work falls under the GNU Public License !!**

## ProjectCustomization
This project adds some customization to ReP_AL-3D-Lawn-Mower:
- Add italian translations, you can choose install language from MEGA_V6.31.ino file
- Fix DFRobot_QMC5883 library, in the original version the method readRegister16 could loop the robot!!
- Add motors pinMode inizialization

## Project Folder

### Libraries
All the libraries used for the Arduino Code can be found in Libraries folder.
Note: i improved the "DFRobot_QMC5883-master" library, i replace it with "DFRobot_QMC5883-otto"

### Mower Arduino Code
This folder contains the mower software :  
- MEGA: Is the main mower source, needs to be uploaded to the Arduino MEGA
- NANO: Needs to be uploaded to the Arduino NANO connect to Arduni MEGA, note: i needed to use the "old bootloader" option in the Arduino IDE to get - the sketch to upload to the NANO.

### Perimeter Wire Transmitter
Contains the wire transmitter source, Sender_station_V1 needs to be uploaded to the Arduino in the perimeter wire transmitter.

### ReP_AL Mower Step Files
CAD Data is fouind in the ReP_AL Mower STEP Files Folder

### Test Sketches
They are designed to test the indiviual components in your build to see if they are working. 
Ihe idea is to debug the components indicudually so you are faster in completing the project.

## Circuit and Design
For the robot design an circuit overview refer to ReP_AL project: https://repalmakershop.com/ .

