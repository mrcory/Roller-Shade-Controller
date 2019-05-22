//Config stuff

//Set positions 1-4 in steps for stop locations. 
//Scaled is figured for 10 positions between 1 and 4
//Position:     1    2     3     4   Scaled
long shade[5] = {0,20500,41000,60000,0};

//Speed Settings in Steps/Second
#define stepperSpeed 950.0 //Max Speed
#define stepperAccel 200.0  //Acceleration Rate

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
#define mtrPin1 D8//D5
#define mtrPin2 D6//D7
#define mtrPin3 D7//D6
#define mtrPin4 D5//D8

//Button Config
#define buttonEnable true //True to enable button controls
#define buttonPin A0 //Analog pin for buttons

//AnalogRead values for buttons
#define upVal 964
#define dnVal 875
#define rsVal 1024

#define ANALOGBUTTONS_SAMPLING_INTERVAL 10

//Feedback for button presses
#define lightEnable true //Allow control of LED
#define lightMode 1 //0-PWM 1-FastLED
#define ledPin D4 //Feedback LED

#define alternateFunction false //Enable alternate use of the reset button


int ledBrightness = 2;
int ledButtonBrightness = 50; //Brightness to use when using rst button to turn on light.




//Extra Stuff
#define rtcBlynk false
#define cmds true //After setting up positions, cmdArduino can be disabled
