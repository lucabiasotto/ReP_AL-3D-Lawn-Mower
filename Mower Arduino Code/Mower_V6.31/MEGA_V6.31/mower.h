/**
 * In this class there are pin setup for Arduino MEGA
 */

#include <DS1302.h>

//Perimeter Wire Pins
#define pinPerimeterLeft A5 //perimeter sensor pin
#define pinPerimeterRight A4 // leave open
#define pinLED LED_BUILTIN

//Real Time Clock Pins
const int kCePin   = 29; // RST
const int kIoPin   = 30; // DAT
const int kSclkPin = 31; // CLK
DS1302 rtc(kCePin, kIoPin, kSclkPin);


//Sonar Setup for Front Sonars 1-3
//S1
#define echoPin1 34
#define trigPin1 35
//S2
#define echoPin2 36
#define trigPin2 37
//S3
#define echoPin3 38
#define trigPin3 39

//Membrane Switch
#define Start_Key 50 //connect wire 1 to pin 2
#define Plus_Key 51  //connect wire 2 to pin 3
#define Minus_Key 52 //connect wire 3 to pin 4
#define Stop_Key 53  //connect wire 4 to pin 5

//Pin Setup for the wheel Motor Bridge Controller
//Motor A
#define ENAPin 7 // EN Pins need a digital pin with PWM
#define IN1Pin 6 // IN Pins dont need digital PWM 
#define IN2Pin 5
//Motor B
#define ENBPin 2                // EN Pins need a digital pin with PWM
#define IN3Pin 4                // IN Pins dont need digital PWM
#define IN4Pin 3

//Motor Blades
#define RPWM 8
#define L_EN 9
#define R_EN 10

//Relay Switch
#define Relay_Motors 24// be careful that you really use PIN24. The order is sometimes labelled so it looks like 24 is actually 22.

//Pin per controllare se usare o no il cavo
#define USE_CABLE_PIN 11 
