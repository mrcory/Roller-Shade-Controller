//Config stuff

//Set positions 1-4 in steps for stop locations. Scaled is figured for 10 positions between 1 and 4
//Position:     1    2     3     4   Scaled
long shade[5] = {0,20500,41000,61500,0};

//Speed Settings in Steps/Second
#define stepperSpeed 1000.0 //Max Speed
#define stepperAccel 212.0  //Acceleration Rate

int connectTimeout = 100; // How many attempts can we make before giving up on Blynk
#define blynkRtcInterval = 30; //How often to sync the time

#include "blynk.h" //Contains Blynk login

//Comment the below line to disable Blynk Serail feedback
#define BLYNK_PRINT Serial

//------Timer
int stepperPos[] = {0,6000};
int stepperTime[2][2] = {};

const int eepromSize = 512;

const int stepPerRev = 4096;
//Declare motor pins
#define mtrPin1 D5
#define mtrPin2 D7
#define mtrPin3 D6
#define mtrPin4 D8

//Button Config
#define buttonEnable true //True to enable button controls
#define buttonPin A0 //Analog pin for buttons

//AnalogRead values for buttons
#define upVal 990
#define dnVal 903
#define rsVal 1024

#define ANALOGBUTTONS_SAMPLING_INTERVAL 10

//Feedback for button presses
#define lightEnable true //Allow control of LED
#define lightMode 1 //0-PWM 1-FastLED
#define ledPin D1 //Feedback LED




int ledBrightness = 2; //Not used yet



//Extra Stuff
#define alternateFunction false //Enable alternate use of the reset button

#define rtcBlynk false
#define cmds true //After setting up positions, cmdArduino can be disabled
