/*Roller Shade Control
 * Created by: Cory McGahee
 * Free for non-commercial use only
 * 
 * Designed for use with my 3D printed roller shade.
 * https://www.thingiverse.com/thing:3628982
 * 
 * 
 * Serial Commands
 * move <val> | Absolute move in steps
 * pos  <val> | Move to set positions
 * rst        | Reset home position to 0
 */

//version 2.5.0
//Latest tested : 
const byte configVersion = 1; //This will prevent loading an old config from EEPROM

#include <AccelStepper.h>
#include <EEPROM.h>

long currentDistance = 0;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Ethernet.h>
#include "config.h"
#include "timer.h"

//FastLED Setup
#include <FastLED.h>
FASTLED_USING_NAMESPACE


CRGB leds[NUM_LEDS];


#if cmds //Enable cmdArduino
  #include <Cmd.h>
#endif

byte lastPosition = 2; //Storing a starting value



//Configure AccelStepper
#ifdef stepperMini
    AccelStepper stepper(ctrlType, mtrPin4,mtrPin3,mtrPin2,mtrPin1);
#endif
 
#ifdef stepperNema
  AccelStepper stepper = AccelStepper(ctrlType, stepPin, dirPin);
#endif

bool configSaveFlag = false;
bool firstRun = true;
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
bool pulse = false; //Basic effect

bool lightOn = false;
bool lightOld = true; 

int currentColor[3] = {255,255,255};
int pulseSpeed = 2;

bool pwmOn = false;
bool pwmOld = false; 
int  oldBrightness = ledBrightness;
int  pwmBrightness = 0;
int  pwmBrightnessOut = 0;




#include "wifi.h"
#include "functions.h"
#include "lightcontrol.h"


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

    if (lightMode == 1) {
      lightOn = !lightOn;
      ledBrightness = ledButtonBrightness;
    }

    if (lightMode == 0) {
      pwmOn = !pwmOn;
      pwmBrightness = ledButtonBrightness;
    }
    
    ledTurn(1);
  }

  void resetHold() {
    Serial.println("Rst Hold");
    stepper.setCurrentPosition(0);
    ledTurn(1);
  }

  //Create tha actual buttons
  AnalogButtons buttons(buttonPin, INPUT,buttonDebounce,buttonMargin);
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
  timerSetup();
  analogWrite(PWM_PIN,0);
  
  if (lightMode == 1) { //Use FastLED if selected
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS);
  }

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

  if (EEPROM.read(0) == 1 && configMatch()) { //If flagged in EEPROM; we load our current position
    configLoad();
  }


  #if rtcBlynk
    //Using Blynk to get the time
    RTC.begin();
    setSyncInterval(blynkRtcInterval);
  #endif
  
  stepper.setMaxSpeed(stepperSpeed[2]);
  stepper.setAcceleration(stepperAccel);

  stepper.setCurrentPosition(savedPosition);

  motorOff();

  
  
  Serial.print(F("Loaded | Position S|C : ")); Serial.print(savedPosition); Serial.print("|"); Serial.println(stepper.currentPosition());
  if (lightMode == 1) { //Use FastLED if selected
    FastLED.setBrightness(ledBrightness);
  }
} //END SETUP


void loop() {


  oldBrightness = ledBrightness;
  currentDistance = stepper.distanceToGo();
  checkInvert();
   
  if (ledFeedback == true) {
    ledFeedbackf();
  }

  #if cmds
    cmdPoll();
  #endif
  
  #if buttonEnable
    buttons.check(); //Check for button presses
  #endif


  //If not moving, run extra features
  if (stepper.distanceToGo() == 0) {
    if (timer(0,blynkRefresh)) { //Run blyk every other cycle
         blynkRun();        //Only run blynk when the stepper is not active
    }
   

    if (setHome == true) {
      resetHold(); //Set current position as home
      setHome = false;
    }

    lightControl();

    if (lightMode == 1) { //Use FastLED if selected
      if (ledBrightness != oldBrightness) {
        FastLED.setBrightness(ledBrightness);
      }
  
      FastLED.show();
   }

   if (lightMode == 0) {
    if (pwmBrightness != pwmBrightnessOut) {
      analogWrite(PWM_PIN,pwmBrightnessOut);
    }
   }
  }

  #ifdef stepperNema
    brakeCheck();
  #endif
  
  stepper.run(); //AccelStepper runs here
  
  motorControl();

  if (configSaveFlag == true) {
    configSave();
  }
  
} //END LOOP
