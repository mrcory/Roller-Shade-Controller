//Config stuff
int motorPos = 0;

//Set positions 1-4 in steps for stop locations. Scaled is figured for 10 positions between 1 and 4
//Position:     1   2     3     4   Scaled
int shade[5] = {0,20500,41000,61500,0};


int connectTimeout = 100; // How many attempts can we make before giving up on Blynk
int blynkRtcInterval = 30; //How often to sync the time

#include "blynk.h" //Contains Blynk login

#define BLYNK_PRINT Serial

//------Timer

int stepperPos[] = {0,6000};
int stepperTime[2][2] = {};

const int eepromSize = 512;

const int stepPerRev = 4096;
//Stepper stepper (stepPerRev,D1,D2,D3,D4); //Declare motor pins
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

#define tickVal 10

//Feedback for button presses
#define ledPin D1
#define ledBrightness //Not used yet
