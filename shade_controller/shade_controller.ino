/*Roller Shade Control
 * Created by: Cory McGahee
 * Free for non-commercial use only
 * 
 * Home Assistant Code Based From:
 * https://github.com/dawidchyrzynski/arduino-home-assistant/blob/main/examples/fan/fan.ino
 * 
 * Designed for use with my 3D printed roller shade.
 * https://www.thingiverse.com/thing:3628982
 * 
 * Serial Commands
 * move <val> | Absolute move in steps
 * pos  <val> | Move to set positions
 * rst        | Reset home position to 0
 */

#define BLYNK_PRINT Serial

//version 3.0.0
const int configVersion = 4;
int   safeConfigVersion = 3; //The oldest version of config that can
                             //be updated from.

#include "structs.h"

bool firstRun = true;
bool resetFlag = false;
bool configLoadFlag = false;
bool configSaveFlag = false;

#include <AccelStepper.h>
#include <EEPROM.h>

long currentDistance = 0;

#include <ESP8266WiFi.h>
#ifdef Blynkenable0
  #include <BlynkSimpleEsp8266.h>
#endif
#include <Ethernet2.h>
#include "config.h"




//ArduinoOTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "ota.h"

//wifiManager
#ifdef wifiManagerEnable
  #include <WiFiManager.h> 
  #include <ESP8266WebServer.h>
#endif

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
  AccelStepper stepper(ctrlType, mtrPin1,mtrPin2,mtrPin3,mtrPin4);
#endif
 
#ifdef stepperNema
  AccelStepper stepper = AccelStepper(ctrlType, stepPin, dirPin);
#endif


long int stepPosition = 0;
int connectAttempt = 0;
long int posNow = 0;
bool moving = false; //Flag
bool myReset = false; //Flag
bool setHome = false; //Flag
long int savedPosition = 100;
bool moveUp = false;
int motorPos = 0;
bool goDown = true;

unsigned long ledTimer;
bool ledFeedback = false;
byte ledMode = 0;
bool pulse = false; //Basic effect

bool lightOn = false;
bool lightOld = true; 
int currentColor[3] = {255,255,255};
int pulseSpeed = 2;

pwmStruct pwm = {
  ledBrightness, //old
  0, //set
  false, //on
  false, //onOld
  0 //out
};

#ifdef Blynkenable
  #include "wifi.h"
#endif

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

    if (lightMode == 1) {
      lightOn = !lightOn;
      ledBrightness = ledButtonBrightness;
    }

    if (lightMode == 0) {
      pwm.on = !pwm.on;
      pwm.set = ledButtonBrightness;
    }
    
    ledTurn(1);
  }

  void resetHold() {
    Serial.println("Rst Hold");
    stepper.setCurrentPosition(0);
    configSave();
    ledTurn(1);
  }

  //Create tha actual buttons
  AnalogButtons buttons(buttonPin, INPUT,buttonDebounce,buttonMargin);
  Button up = Button(upVal, &upClick, &upHold, 1000, 5000);
  Button down = Button(dnVal, &downClick, &downHold, 1000, 5000);
  Button rst = Button(rsVal, &resetClick, &resetHold, 2500, 5000);
  
#endif 

#ifdef Blynkenable
  #if rtcBlynk //Enable the RTC feature from Blynk
    //Blynk RTC
    #include <WidgetRTC.h>
    WidgetRTC RTC;
    BlynkTimer timer1;
  #endif
#endif

#include "lightcontrol.h"

void setup() {
  Serial.begin(74880);
  delay(50);

  #ifdef wifiManagerEnable
    // WiFiManager intialization.
    WiFiManager wifi; 
  #endif


  
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

  #ifdef wifiManagerEnable
    // Create AP, if necessary
    wifi.autoConnect(myHostName);
  
    // Set WIFI module to STA mode
    WiFi.mode( WIFI_STA );
  #endif
  

  #ifdef Blynkenable
    blynkConfig();
  #endif
  setupOTA();

  EEPROM.begin(eepromSize); //Initialize the EEPROM with our selected size

  if (EEPROM.read(0) == 1 && returnConfigVersion() == configVersion) { //If flagged in EEPROM; we load our current position
    configLoad();
    Serial.println("Pass Config Version Check");
  } else {
    Serial.println("Fail Config Version Check, Reseting To Default");
    configSave();
    
  }

  #ifdef Blynkenable
    #if rtcBlynk
      //Using Blynk to get the time
      RTC.begin();
      setSyncInterval(blynkRtcInterval);
    #endif
  #endif
  
  stepper.setMaxSpeed(mSpeed.up);
  stepper.setAcceleration(mSpeed.accel);

  stepper.setCurrentPosition(savedPosition);

  motorOff();

  
  
  Serial.print(F("Loaded | Position S|C : ")); Serial.print(savedPosition); Serial.print("|"); Serial.println(stepper.currentPosition());
  if (lightMode == 1) { //Use FastLED if selected
    FastLED.setBrightness(ledBrightness);
  }

} //END SETUP


void loop() {


  pwm.old = pwm.set;
  currentDistance = stepper.distanceToGo();
   
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
    
    #ifdef Blynkenable
      blynkRun(); //Only run blynk when the stepper is not active
    #endif
    
    if (setHome == true) {
      resetHold(); //Set current position as home
      setHome = false;
    }

    lightControl();

    if (lightMode == 1) { //Use FastLED if selected
      if (ledBrightness != pwm.old) {
        FastLED.setBrightness(ledBrightness);
      }
  
      FastLED.show();
   }

   if (lightMode == 0) {
    if (pwm.set != pwm.old) {
      analogWrite(PWM_PIN,pwm.set);
      pwm.old = pwm.set;
    }
   }
  }

  #ifdef stepperNema
    brakeCheck();
  #endif
  
  stepper.run(); //AccelStepper runs here
  motorControl();
  ArduinoOTA.handle();

    if (resetFlag == true) {
      ESP.reset();
    }
  
    if (configLoadFlag == true) {
      configLoad();
      configLoadFlag = false;
    }
  
    if (configSaveFlag == true) {
      configSave();
      configSaveFlag = false;
    }

} //END LOOP
