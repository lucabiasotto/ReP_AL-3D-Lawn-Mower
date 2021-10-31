#include "rainUtils.h"
#include "robot.h"

void readRainSensor() { //TODO perchè non far tornare un bel boleano?
    if (Rain_Sensor_Installed == 1) {
        robot.rainDetected = 0;  //TODO analogRead(A3); o digital read? è 1 o 0 leggendo qua
        if (robot.rainDetected == 1) {
            robot.rainHitDetected = robot.rainHitDetected + 1;
        }else if (robot.rainHitDetected > 0) {
            robot.rainHitDetected = robot.rainHitDetected - 1;
        }
    }
}
