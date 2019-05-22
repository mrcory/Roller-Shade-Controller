/*Roller Shade Control
 * Created by: Cory McGahee
 * Free for non-commercial use only
 * 
 * Designed for use with my 3D printed roller shade. (Not yet published)
 * 
 * 
 * Serial Commands
 * move <val> | Absolute move in steps
 * pos  <val> | Move to set positions
 * rst        | Reset home position to 0
 */

//version 1.2.0

#include <AccelStepper.h>
#include <EEPROM.h>

#define HALFSTEP 8 //This is for AccelStepper

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Ethernet.h>
#include "config.h"

//FastLED Setup
#include <FastLED.h>
FASTLED_USING_NAMESPACE
#define NUM_LEDS 9

#define DATA_PIN    3
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];


#if cmds //Enable cmdArduino
  #include <Cmd.h>
#endif

byte lastPosition = 2; //Storing a starting value

//Configure AccelStepper
AccelStepper stepper(HALFSTEP, mtrPin1,mtrPin2,mtrPin3,mtrPin4);



long int stepPosition = 0;
int connectAttempt = 0;
long int posNow = 0;
bool moving = false; //Flag
bool myReset = false; //Flag
bool setHome = false; //Flag
long int savedPosition = 100;
bool moveUp = false;
int motorPos = 0;

unsigned long ledTimer;
bool ledFeedback = false;
byte ledMode = 0;

bool lightOn = false;
bool lightOld = true; 
int oldBrightness = ledBrightness;


#include "wifi.h"
#include "functions.h"


#if buttonEnable //Enable physical buttons

  #include <AnalogButtons.h>

 //Here we are setting up the fuctions that will be called via the physical buttons.
  void upClick() {
    Serial.println("Up Click");
    if (lastPosition > 1) {
      lastPosition--;
      motorPos = lastPosition;
      Serial.println(motorPos);
      ledTurn(0);
    } else {
      ledTurn(2);
    }
  }

  void upHold() {
    Serial.println("Up Hold");
    motorPos = 1;
    lastPosition = 1;
    ledTurn(1);
  }

  void downClick() {
    Serial.println("Dn Click");
    if (lastPosition < 4) {
      lastPosition++;
      motorPos = lastPosition;
      Serial.println(motorPos);
      ledTurn(0);
    } else {
      ledTurn(2);
    }
  }

  void downHold() {
    Serial.println("Dn Hold");
    motorPos = 4;
    lastPosition = 4;
    ledTurn(1);
  }

  void resetClick() {
    Serial.println("Rst Click");
    lightOn = !lightOn;
    ledBrightness = ledButtonBrightness;
    ledTurn(1);
  }

  void resetHold() {
    Serial.println("Rst Hold");
    stepper.setCurrentPosition(0);
    ledTurn(1);
  }

  //Create tha actual buttons
  AnalogButtons buttons(buttonPin, INPUT,4,buttonMargin);
  Button up = Button(upVal, &upClick, &upHold, 1000, 5000);
  Button down = Button(dnVal, &downClick, &downHold, 1000, 5000);
  Button rst = Button(rsVal, &resetClick, &resetHold, 2500, 5000);
  
#endif 

#if rtcBlynk //Enable the RTC feature from Blynk
  //Blynk RTC
  #include <WidgetRTC.h>
  WidgetRTC RTC;
  BlynkTimer timer1;
#endif



void setup() {
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  
  #if buttonEnable
    buttons.add(up);
    buttons.add(down);
    buttons.add(rst);
  #endif

  #if cmds //Define our serial commands if enabled
    cmdInit(&Serial);
    cmdAdd("move",manualMove);
    cmdAdd("rst",homePos);
    cmdAdd("pos",pos);
  #endif


  

  Serial.begin(115200);
  Serial.println("");
  delay(50);
  blynkConfig();

  EEPROM.begin(eepromSize); //Initialize the EEPROM with our selected size

  if (EEPROM.read(0) == 1) { //If flagged in EEPROM; we load our current position
    configLoad();
  }


  #if rtcBlynk
    //Using Blynk to get the time
    RTC.begin();
    setSyncInterval(blynkRtcInterval);
  #endif
  
  stepper.setMaxSpeed(stepperSpeed);
  stepper.setAcceleration(stepperAccel);

  stepper.setCurrentPosition(savedPosition);

  motorOff();

  
  
  Serial.print(F("Loaded | Position S|C : ")); Serial.print(savedPosition); Serial.print("|"); Serial.println(stepper.currentPosition());

  FastLED.setBrightness(ledBrightness);
  
} //END SETUP


void loop() {


  oldBrightness = ledBrightness;
   
  if (ledFeedback == true) {
    ledFeedbackf();
  }

  #if cmds
    cmdPoll();
  #endif
  
  #if buttonEnable
    buttons.check(); //Check for button presses
  #endif


  //If not moveing, run extra features
  if (stepper.distanceToGo() == 0) {
    blynkRun(); //Only run blynk when the stepper is not active

    if (setHome == true) {
      setHome = false;
    }

    lightControl();
  
    if (ledBrightness != oldBrightness) {
      FastLED.setBrightness(ledBrightness);
      FastLED.show();
    }
  }
  
  stepper.run(); //AccelStepper runs here
  
  moveNow();
  
} //END LOOP



void lightControl() {
  
  if (lightOn == true && lightOld != lightOn) {
    fill_solid(leds,NUM_LEDS,CRGB::White);
    FastLED.show();
  }

  if (lightOn == false && lightOld != lightOn) {
    fill_solid(leds,NUM_LEDS,CRGB(0,0,0));
    FastLED.show();
  }
  lightOld = lightOn;
}


