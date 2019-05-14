/*Roller Shade Control
 * Created by: Cory McGahee
 * Free for non-commercial use only
 * 
 * Designed for use with my 3D printed roller shade. (Not yet published)
 */

//version 1.0

#include <AccelStepper.h>
#include <EEPROM.h>

#define HALFSTEP 8

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Ethernet.h>
#include <Cmd.h>
#include "config.h"


AccelStepper stepper(HALFSTEP, mtrPin1,mtrPin2,mtrPin3,mtrPin4);

long int stepPosition = 0;
int connectAttempt = 0;
long int posNow = 0;
long int moveTarget = 0;
bool moving = false;
bool myReset = false;
bool setHome = false;
long int savedPosition = 100;

bool moveUp = false;

#include "functions.h"
#include "wifi.h"

//Blynk RTC
#include <WidgetRTC.h>
WidgetRTC RTC;
BlynkTimer timer1;

void setup() {
  cmdInit(&Serial);
  cmdAdd("move",manualMove);
  cmdAdd("rst",homePos);


  

  Serial.begin(115200);
  Serial.println("");
  delay(50);
  blynkConfig();

  EEPROM.begin(eepromSize);

  if (EEPROM.read(0) == 1) {
    configLoad();
  }


  //Using Blynk to get the time
  RTC.begin();
  setSyncInterval(blynkRtcInterval);

  stepper.setMaxSpeed(1050.0);
  stepper.setAcceleration(75.0);
  stepper.setSpeed(600);

  stepper.setCurrentPosition(savedPosition);

  motorOff();

  Serial.print("Loaded | Position S|C : "); Serial.print(savedPosition); Serial.print("|"); Serial.println(stepper.currentPosition());
}


void loop() {
  cmdPoll();
  if (stepper.distanceToGo() == 0) {
    blynkRun(); //Only run blynk when the stepper is not active

    if (setHome == true) {
      setHome = false;
    }
  }
  
  stepper.run(); //AccelStepper runs here
  moveNow();
}



void blynkConfig (){
  Blynk.config(auth,server,port);
  Blynk.connectWiFi(ssid,pass);
}

void blynkRun() {
  if (Blynk.connected() == true) {
    Blynk.run();
    if (connectTimeout != 0) {
      connectAttempt = 0; //Reset timeout counter if successfully connected
    }
  } else {
    if (connectAttempt < connectTimeout) {
      Serial.print("[BLYNK] Attempting to connect... | Attempt #"); Serial.println(connectAttempt);
      Blynk.connect(3000); //Attempt to connect for 3 seconds.
      if (Blynk.connected() == true) {Serial.println("[BLYNK] Connected!");}
      connectAttempt++; //Increment timeout timer
    }
  }

  timer1.run(); //Blynk RTC
  sendBlynk();
  stepPosition = posNow + stepper.distanceToGo();
}



void motorOff() {
  digitalWrite(mtrPin1,LOW);
  digitalWrite(mtrPin2,LOW);
  digitalWrite(mtrPin3,LOW);
  digitalWrite(mtrPin4,LOW);
  
}

void moveNow() {
  
  if (posNow != moveTarget) {
    stepper.moveTo(moveTarget);
    moving = true;
  }

  if (stepper.distanceToGo() == 0) {
    //posNow = moveTarget;
    moving = false;
    motorOff();   

    if (savedPosition != stepper.currentPosition()) {
      savedPosition = stepper.currentPosition();
      configSave();
      Serial.print("Position S|C : "); Serial.print(savedPosition); Serial.print("|"); Serial.println(stepper.currentPosition());
    }
  }

  if (motorPos > 0) {
    if (motorPos == 1) {stepper.moveTo(shade[0]);}
    if (motorPos == 2) {stepper.moveTo(shade[1]);}
    if (motorPos == 3) {stepper.moveTo(shade[2]);}
    if (motorPos == 4) {stepper.moveTo(shade[3]);}
    if (motorPos == 5) {stepper.moveTo(shade[4]);}
    Serial.println("Blynk Move");
  motorPos = 0;
  }
   
}


void manualMove(int arg_cnt, char **args) {
  stepper.moveTo(cmdStr2Num(args[1], 10));
  Serial.print("Manual Move: "); Serial.println(cmdStr2Num(args[1], 10));
}

void homePos(int arg_cnt, char **args) {
  stepper.setCurrentPosition(0);
}


