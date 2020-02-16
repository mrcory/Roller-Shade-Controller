//Config stuff

/* Speed Settings in Steps/Second
 * You will have to play with these values to find a 
 * reliable setup. I recomend putting a small extra load
 * (lightly pressing a finger against the roller) to 
 * be sure that there is enough strength to lift the shade
 * reliably.
 * 
 * .! This is used for both motor types !.
 */
#define stepperSpeed 1100.0 //Max Speed
#define stepperAccel 175.0  //Acceleration Rate

motorSpeedStruct mSpeed = {
  1000, //Up Speed
  1000, //Down Speed
  175   //Acceleration
};



/* Drive motor selection.
 * You have choice between 2 style motors. 
 * 
 * 28BYJ-48 : 5v or 12v | Though slow, these motors are cheap.
 * The 5v has the advantage of being able to receive power from a
 * power bank. This could allow operation when your electricity goes out.
 * 
 * Nema 17 : 12v        | This motor should be faster, but will require a
 * brake or to remain powered to hold its position.
 * 
 * 28BYJ will be controlled by steps, while Nema will be controlled by degrees.
 * 
 * Uncomment the appropriate line below. Edit the config at the top of selected.
 */
#include "stepperMini.h"
//#include "stepperNema.h"

int connectTimeout = 100;      // How many attempts can we make before giving up on Blynk
#define blynkRtcInterval = 30; // How often to sync the time. (Would be used for timers)

#include "blynk.h" //Contains Blynk login

//Comment the below line to disable Blynk Serail feedback
#define BLYNK_PRINT Serial

//------Timer
int stepperPos[] = {0,6000};
int stepperTime[2][2] = {};
//------Not currently used

/* Set size for the eeprom.
 * 512 is way more than will be needed.
 * All that is being stored there is position data
 * after each move is completed.
 */
const int eepromSize = 512;



//Button Config
#define buttonEnable true //True to enable button controls
#define buttonPin A0      //Analog pin for buttons

//AnalogRead values for buttons
#define upVal 895
#define dnVal 980
#define rsVal 1024

/*  The margin that analogButtons uses to determine
 *  if a button has been pressed. Default is 20, but
 *  I use 30. As long as there is plenty of gap between 
 *  values set above, this can be set larger.
 */
#define buttonMargin 30
#define buttonDebounce 3 //Debounce for buttons. Lower causes faster trigger.
#define ANALOGBUTTONS_SAMPLING_INTERVAL 10

//Feedback for button presses
#define ledPin D3        //Feedback LED

/*  This will allow control of an accessory light via
 *  PWM or FastLED.
 *  If using FastLED controlled lights, be careful of
 *  the number you use. Always calcualte the max possible
 *  power consumption and add a safety margin.
 */
#define alternateFunction false //Enable alternate use of the reset button
#define lightMode 0             //0-PWM 1-FastLED | PWM is currently not implemented.

/*  Light accessory settings (Not the feedback LED)
 *  Using FastLED to run some WS2812 
 *  The data pin for the WS2812 is set in the main ino 
 *  around line 31
*/
int ledBrightness = 0;        //Starting brightness
int ledButtonBrightness = 50; //Brightness to use when using rst button to turn on light.
int pulseMax = 250;           //Max brightness for pulse mode. Max 250 or it will loop to near 0
                              //Can be set in Blynk

//FastLED Settings
#define NUM_LEDS 9         //Number of attached LEDs
#define DATA_PIN    D1     //Communication pin for the LEDs | Same pin will be used for either LED control
#define LED_TYPE    WS2812 //LED controller
#define COLOR_ORDER GRB    //Order to send colors

//PWM Light Settings
#define PWM_PIN     D2     //Rev.0 PCB uses D2


//Extra Stuff
#define rtcBlynk false
#define cmds true //After setting up positions, cmdArduino can be disabled

//OTA Settings
#define updatePassword "admin"
#define myHostName     "Shade"
