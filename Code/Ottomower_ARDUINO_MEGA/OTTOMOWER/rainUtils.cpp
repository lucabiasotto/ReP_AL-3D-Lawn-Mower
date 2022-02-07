#include "rainUtils.h"
#include "robot.h"
#include "adcman.h"

void readRainSensor() { //TODO perchè non far tornare un bel boleano?
    if (RAIN_SENSOR_INSTALLED == 1) {
        robot.rainDetected = ADCMan.read(RAIN_PIN);//digitalRead(RAIN_PIN);

        if (robot.rainDetected > 80) { //MAX 127
            if(robot.rainHitDetected < 127){
                robot.rainHitDetected = robot.rainHitDetected + 1;
            }

        }else {
            robot.rainHitDetected = 0;
        }
    }
}
