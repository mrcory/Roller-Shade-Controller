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

//version 1.1.0

#include <AccelStepper.h>
#include <EEPROM.h>



#define HALFSTEP 8

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Ethernet.h>
#include <Cmd.h>
#include "config.h"



byte lastPosition = 2;

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

unsigned long ledTimer;
bool ledOn = false;
byte ledMode = 0;

#include "functions.h"
#include "wifi.h"


#if buttonEnable

  #include <AnalogButtons.h>

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
  }

  void resetHold() {
    Serial.println("Rst Hold");
    stepper.setCurrentPosition(0);
    ledTurn(1);
  }

  AnalogButtons buttons(buttonPin, INPUT);
  Button up = Button(upVal, &upClick, &upHold, 1000, 5000);
  Button down = Button(dnVal, &downClick, &downHold, 1000, 5000);
  Button rst = Button(rsVal, &resetClick, &resetHold, 1000, 5000);
  
#endif 

//Blynk RTC
#include <WidgetRTC.h>
WidgetRTC RTC;
BlynkTimer timer1;

void setup() {

  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  
  #if buttonEnable
    buttons.add(up);
    buttons.add(down);
    buttons.add(rst);
  #endif
  
  cmdInit(&Serial);
  cmdAdd("move",manualMove);
  cmdAdd("rst",homePos);
  cmdAdd("pos",pos);


  

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

  stepper.setMaxSpeed(1000.0);
  stepper.setAcceleration(212.0);

  stepper.setCurrentPosition(savedPosition);

  motorOff();

  
  
  Serial.print(F("Loaded | Position S|C : ")); Serial.print(savedPosition); Serial.print("|"); Serial.println(stepper.currentPosition());
}


void loop() {
  if (ledOn == true) {
    ledFeedback();
  }
  cmdPoll();
  #if buttonEnable
    buttons.check(); //Check for button presses
  #endif
  
  if (stepper.distanceToGo() == 0) {
    blynkRun(); //Only run blynk when the stepper is not active

    if (setHome == true) {
      setHome = false;
    }
  }
  
  stepper.run(); //AccelStepper runs here
  moveNow();
  //Serial.println(analogRead(A0));
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
      Serial.print(F("[BLYNK] Attempting to connect... | Attempt #")); Serial.println(connectAttempt);
      Blynk.connect(3000); //Attempt to connect for 3 seconds.
      if (Blynk.connected() == true) {Serial.println(F("[BLYNK] Connected!"));}
      connectAttempt++; //Increment timeout timer
    }
  }

  timer1.run(); //Blynk RTC
  sendBlynk();
  stepPosition = posNow + stepper.distanceToGo();
}


//Write motor pins low to be sure the motor is unpowered
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
      Serial.print(F("Position S|C : ")); Serial.print(savedPosition); Serial.print("|"); Serial.println(stepper.currentPosition());
    }
  }

  //Blynk will set a vale to motorPos that is used to set where the motor should run to
  if (motorPos > 0) {
    if (motorPos == 1) {stepper.moveTo(shade[0]);}
    if (motorPos == 2) {stepper.moveTo(shade[1]);}
    if (motorPos == 3) {stepper.moveTo(shade[2]);}
    if (motorPos == 4) {stepper.moveTo(shade[3]);}
    if (motorPos == 5) {stepper.moveTo(shade[4]);}
    Serial.println(F("Blynk Move"));

    lastPosition = motorPos; //Before reseting motorPos, save a copy to lastPosition
    motorPos = 0; //Reset motorPos
  }
   
}


//Function used to control the motor position via Serial
void manualMove(int arg_cnt, char **args) {
  stepper.moveTo(cmdStr2Num(args[1], 10));
  Serial.print(F("Manual Move: ")); Serial.println(cmdStr2Num(args[1], 10));
}

//Set home position via Serial
void homePos(int arg_cnt, char **args) {
  stepper.setCurrentPosition(0);
}

//Move to position via Serial
void pos(int arg_cnt, char **args) {
  if (cmdStr2Num(args[1],10) > 0 && cmdStr2Num(args[1],10) < 5) {
    motorPos = cmdStr2Num(args[1],10);
    Serial.print("Target Position: "); Serial.println(cmdStr2Num(args[1],10));
    
  } else {
    Serial.println("Invalid Input. (1-4 are valid.)");
  }
}



