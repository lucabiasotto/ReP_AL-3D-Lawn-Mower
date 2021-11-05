#include <Arduino.h>
#define Console Serial

void motorsSetFullSpeed();
void motorsSetPinsToGoForwards();
void motorsSetPinsToGoBackwards();
void motorsStopWheelMotors();
void motorsSetPinsToTurnLeft();
void motorsSetPinsToTurnRight();
void motorsetTurnSpeed(int turnSpeedCompensation);
void motorsActivateSpinBlades();
void motorsStopSpinBlades();
void motorsSetDynamicSteeringSpeed();

