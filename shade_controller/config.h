//Config stuff

//Set positions 1-4 in steps for stop locations. 
//Scaled is figured for 10 positions between 1 and 4
//Position:     1    2     3     4   Scaled
long shade[5] = {0,20500,41000,60000,0};

//Speed Settings in Steps/Second
#define stepperSpeed 950.0 //Max Speed
#define stepperAccel 200.0  //Acceleration Rate

int connectTimeout = 100;      // How many attempts can we make before giving up on Blynk
#define blynkRtcInterval = 30; //How often to sync the time

#include "blynk.h" //Contains Blynk login

//Comment the below line to disable Blynk Serail feedback
#define BLYNK_PRINT Serial

//------Timer
int stepperPos[] = {0,6000};
int stepperTime[2][2] = {};
//------Not currently used

/*Set size for the eeprom.
 * 512 is way more than will be needed.
 * All that is being stored there is position data
 * after each move is completed.
 */
const int eepromSize = 512;

//Declare motor pins
#define mtrPin1 D8//D5
#define mtrPin2 D6//D7
#define mtrPin3 D7//D6
#define mtrPin4 D5//D8

const int stepPerRev = 4096; //Steps needed to make 1 revolution

//Button Config
#define buttonEnable true //True to enable button controls
#define buttonPin A0      //Analog pin for buttons

//AnalogRead values for buttons
#define upVal 964
#define dnVal 875
#define rsVal 1024

/* The margin that analogButtons uses to determine
 *  if a button has been pressed. Default is 20, but
 *  I use 30. As long as there is plenty of gap between 
 *  values set above, this can be set larger.
 */
#define buttonMargin 30
#define buttonDebounce 3 //Debounce for buttons. Lower causes faster trigger.

#define ANALOGBUTTONS_SAMPLING_INTERVAL 10

//Feedback for button presses
#define lightEnable true //Allow control of LED
#define ledPin D4        //Feedback LED

/* This will allow control of an accessory light via
 *  PWM or FastLED.
 *  If using FastLED controlled lights, be careful of
 *  the number you use. Always calcualte the max possible
 *  power consumption and add a safety margin.
 */
#define alternateFunction false //Enable alternate use of the reset button
#define lightMode 1             //0-PWM 1-FastLED | PWM is currently not implemented.

/* Light accessory settings (Not the feedback LED)
 *  Using FastLED to run some WS2812 
 *  The data pin for the WS2812 is set in the main ino 
 *  around line 31
*/
int ledBrightness = 2;        //Starting brightness
int ledButtonBrightness = 50; //Brightness to use when using rst button to turn on light.

//FastLED Settings
#define NUM_LEDS 9         //Number of attached LEDs
#define DATA_PIN    D1     //Communication pin for the LEDs
#define LED_TYPE    WS2812 //LED controller
#define COLOR_ORDER GRB    //Order to send colors


//Extra Stuff
#define rtcBlynk false
#define cmds true //After setting up positions, cmdArduino can be disabled
