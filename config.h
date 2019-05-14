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
