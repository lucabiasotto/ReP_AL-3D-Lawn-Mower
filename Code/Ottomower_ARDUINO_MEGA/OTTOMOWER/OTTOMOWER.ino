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

unsigned int resetCount __attribute__((section(".noinit")));

/**
   Function for reset arduino
*/
void (*reset)(void) = 0;

void setup() {
    Serial.begin(115200);

    //reset the mower when powered because some time MPU6050 some time does not work
    if (resetCount == 1) {
        // 1 is the magic number used to flag when i'm after a reset
        Serial.println("AFTER RESET");
    } else {
        //i need to do a reset because sometimes the MPU6050 not works at first boot
        Serial.println("FIRST POWER UP");
        resetCount = 1;
        delay(500);
        reset();
    }
    resetCount = 1;

    robot.setup();
}

void loop() {
    robot.loop();
}
