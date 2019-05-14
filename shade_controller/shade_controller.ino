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
bool moving = false; //Flag
bool myReset = false;
bool setHome = false; //Flag
long int savedPosition = 100;
long int moveTarget;
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

  stepper.setMaxSpeed(1060.0);
  stepper.setAcceleration(212.0);

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


//We will conigure Blynk with this
void blynkConfig (){
  Blynk.config(auth,server,port);
  Blynk.connectWiFi(ssid,pass);
}

//This will connect or run blynk
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

  //If motor has reached it's target location, run this
  if (stepper.distanceToGo() == 0) {
    motorOff();   

    //After movement is complete, write the current position to flash
    //This will preserve the position of the motor through power loss
    if (savedPosition != stepper.currentPosition()) {
      savedPosition = stepper.currentPosition();
      configSave();
      Serial.print("Position S|C : "); Serial.print(savedPosition); Serial.print("|"); Serial.println(stepper.currentPosition());
    }
  }

  //Blynk will set a vale to motorPos that is used to set where the motor should run to
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


//Function used to control the motor position via Serial
void manualMove(int arg_cnt, char **args) {
  stepper.moveTo(cmdStr2Num(args[1], 10));
  Serial.print("Manual Move: "); Serial.println(cmdStr2Num(args[1], 10));
}

//Set home position via Serial
void homePos(int arg_cnt, char **args) {
  stepper.setCurrentPosition(0);
}
