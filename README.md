# OttoMower

**!! All my work falls under the GNU Public License !!**

## ReP_AL
This is a customization of project: [ReP_AL-3D-Lawn-Mower](https://github.com/REPALphilread/ReP_AL-3D-Lawn-Mower)
This projec is based on the: Mower_V6.31

## ProjectCustomization
This project adds some customization to ReP_AL-3D-Lawn-Mower:
- converted .ino files to class c ++, the project is more organized when there are many files and avoid spaghetti code
- the arduino nano is no longer needed for read volts, amp and raining
- rename varible, methods ecc using global code convention (e.g. uppercase constants, camel methods etc), the code is more readable
- code refactoring, removed consecutive if with opposite logics, used `else` instead :)
- use char or byte or consts whenever possible to reduce memory consumption
- removed management via EEPROM, setup is hardcoded. I extend the life of my Arduino :) 
- add italian translations, you can choose language in `translations.h` file
- use MPU6050 for calculcate pitch,roll,yaw (cheaper than QMC5883L)
- Add motors pinMode inizialization
- removed schedules to replace them with time slots in which the robot can work [WIP not ready yet]
- menu is now more user friendly and menu cose is easy to customize

## Project Folders

### Code
Contains all code you need

#### Libraries
All the libraries used for the Arduino Code can be found in Libraries folder.

#### Ottomower_ARDUINO_MEGA
This folder contains the mower software  
- MEGA: Is the main mower source, needs to be uploaded to the Arduino MEGA
- esp8266: WIP

#### Perimeter_Wire_Transmitter_ARDUINO_UNO Wire Transmitter
Contains the wire transmitter source, Sender_station_V1 needs to be uploaded to the Arduino in the perimeter wire transmitter.

#### Test Sketches
They are designed to test the indiviual components in your build to see if they are working. 
Ihe idea is to debug the components indicudually so you are faster in completing the project.

### STL File
CAD Data for printing mower.

## Docs
Contains circuit overview and other info.

## Missing feature
- read AMP for detect charge is WIP
- rain sensor is WIP
