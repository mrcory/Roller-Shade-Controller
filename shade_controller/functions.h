#define eepromStart 10


byte returnConfigVersion() {
  byte grabbedVersion;
  EEPROM.get(eepromStart,grabbedVersion);
  return grabbedVersion;
}


void configSave() {
  //Position 400 is reserved for the blynk token
  EEPROM.write(0,1); //Flag for autoload
  int i=eepromStart;
  EEPROM.put(i,configVersion);
  i+=sizeof(configVersion);
  EEPROM.put(i,savedPosition);
  i+=sizeof(savedPosition);
  EEPROM.put(i,lastPosition);
  i+=sizeof(lastPosition);
  EEPROM.put(i,stepperAccel);
  i+=sizeof(stepperAccel);
  EEPROM.put(i,stepperSpeed[0]);
  i+=sizeof(stepperSpeed[0]);
  EEPROM.put(i,stepperSpeed[1]);
  i+=sizeof(stepperSpeed[1]);
  EEPROM.put(i,invertMotor[0]);
  i+=sizeof(invertMotor[0]);
  EEPROM.put(i,oldBrightness);
  i+=sizeof(oldBrightness);
  EEPROM.put(i,oldPWMBrightness);
  i+=sizeof(oldPWMBrightness);
  EEPROM.commit();
  Serial.println("Config Saved");
}

void configLoad() {
  int i=eepromStart;
  EEPROM.get(i,configVersion);
  i+=sizeof(configVersion);
  EEPROM.get(i,savedPosition);
  i+=sizeof(savedPosition);
  EEPROM.get(i,lastPosition);
  i+=sizeof(lastPosition);
  EEPROM.get(i,stepperAccel);
  i+=sizeof(stepperAccel);
  EEPROM.get(i,stepperSpeed[0]);
  i+=sizeof(stepperSpeed[0]);
  EEPROM.get(i,stepperSpeed[1]);
  i+=sizeof(stepperSpeed[1]);
  EEPROM.get(i,invertMotor[0]);
  i+=sizeof(invertMotor);
  EEPROM.get(i,oldBrightness);
  i+=sizeof(oldBrightness);
  EEPROM.get(i,oldPWMBrightness);
  i+=sizeof(oldPWMBrightness);
  delay(20);
  Serial.print("Config Loaded | Version "); Serial.println(returnConfigVersion());
  Serial.print("Stepper: Speed|"); Serial.print(stepperSpeed[2]); Serial.print(" Accel:"); Serial.println(stepperAccel);
  Serial.println("Current Position:" + savedPosition);
}



bool configMatch() {
  if (returnConfigVersion() == configVersion) {
    Serial.println("Config Match");
    return true;
  } else {
    Serial.println("Config Mismatch");
    return false;
  }
}


void ledFeedbackf() {

  if (ledMode == 0) {
    if (ledTimer > millis() - 500) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }   
  } 

  if (ledMode == 1) {
    if (ledTimer > millis() - 1500) {
      digitalWrite(ledPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
    }
  }

  if (ledMode == 2) {
    if (ledTimer > millis() - 1000) {
      if ((millis() > ledTimer + 250) && (millis()  < ledTimer + 750)) {
        digitalWrite(ledPin, LOW);
      } else {
        digitalWrite(ledPin, HIGH);
      }
  } else {
        digitalWrite(ledPin, LOW);
   }
}
}

void ledTurn(int _mode) {
    ledTimer  = millis();
    ledFeedback = true;
    ledMode = _mode;
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
    sendBlynk();
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

  #if rtcBlynk
   timer1.run(); //Blynk RTC
 #endif
 
  
  stepPosition = posNow + stepper.distanceToGo();
}


void speedCheck() { //Set speed based oin direction
  if (lastPosition < motorPos) {
    stepper.setMaxSpeed(stepperSpeed[1]); //Down Speed
    Serial.print("Speed set to "); Serial.println(stepperSpeed[1]);
  } else {
    stepper.setMaxSpeed(stepperSpeed[0]); //Up Speed
    Serial.print("Speed set to "); Serial.println(stepperSpeed[0]);
  }
  stepper.setAcceleration(stepperAccel);  //Update acceleration
}


void motorControl() {

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
    speedCheck();
    
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

//Commands for controlling over Serial Monitor
#if cmds
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
#endif

void checkInvert() {
  if (invertMotor[0] != invertMotor[1]) {
    for (int i=1;i<=3;i++) {
      shade[i] = shade[i]*-1;
    }
    invertMotor[1] = invertMotor[0]; //Set change flag
    int _tempHold = stepperSpeed[0];
    stepperSpeed[0] = stepperSpeed[1];
    stepperSpeed[1] = _tempHold;
    configSave();
    Serial.print("Direction ");
      if (invertMotor[1] == true) {
        Serial.println("Inverted"); 
      } else {
        Serial.println("Reverted");
      }
  }
}
