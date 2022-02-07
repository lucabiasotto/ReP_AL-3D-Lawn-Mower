// ReP_AL Lawn Mower Arduino Code
// Please make sure you have installed all the library files to the Arduino libraries folder
// You will need to unzip all the libraries from the GitHuB site.
// Instructions onnhow to do this are available on my webpage
// www.repalmkaershop.com

//Libraries for Perimeter Wire Receiver
#include <Arduino.h>
#include <Wire.h>

#include "drivers.h"
#include "perimeter.h"
#include "robot.h"

//Real Time Clock Pins
const int kCePin = 29;    // RST
const int kIoPin = 30;    // DAT
const int kSclkPin = 31;  // CLK
//TODO RTCDS1302 rtc1302(kCePin, kIoPin, kSclkPin);


void setup() {
    Serial.begin(115200);
    robot.setup();
}

void loop() {
    robot.loop();
}
