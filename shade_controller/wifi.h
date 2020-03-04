bool firstRun = true;

bool resetFlag = false;
bool configLoadFlag = false;
bool configSaveFlag = false;


void sendBlynk() { //Blynk Feedback
  Blynk.virtualWrite(V1,stepper.currentPosition());
  Blynk.virtualWrite(V28,pwm.on);

  if (pwm.old != pwm.set) {
    Blynk.virtualWrite(V27,pwm.set);
  }


  if (firstRun == true) {
    Blynk.virtualWrite(V11,shade[4]);
    Blynk.virtualWrite(V27,pwm.set);
    Blynk.virtualWrite(V30,mSpeed.up);
    Blynk.virtualWrite(V31,mSpeed.accel);
    Blynk.virtualWrite(V34,mSpeed.dn);
    Blynk.virtualWrite(V32,mInvert.set);
    Blynk.virtualWrite(V37,ledButtonBrightness);
    firstRun = false;
  }
}

BLYNK_WRITE(V11) {
  shade[4] = map(param.asInt(),0,10,shade[0],shade[3]);
}

BLYNK_WRITE(V12) {
  myReset = param.asInt();
}

BLYNK_WRITE(V13) {
  setHome = param.asInt();
}

//Controls
BLYNK_WRITE(V20) {
  motorPos = param.asInt();
}

BLYNK_WRITE(V21) {
  lightOn = param.asInt();
}

BLYNK_WRITE(V22) {
  ledBrightness = param.asInt();
}

BLYNK_WRITE(V23) {
  currentColor[0] = param[0].asInt();
  currentColor[1] = param[1].asInt();
  currentColor[2] = param[2].asInt(); 
}

BLYNK_WRITE(V24) {
  pulse = param.asInt();
}

BLYNK_WRITE(V25) {
  pulseSpeed = param.asInt();
}

BLYNK_WRITE(V26) {
  pulseMax = param.asInt();
}

BLYNK_WRITE(V27) {
  pwm.set = param.asInt();
}

BLYNK_WRITE(V28) {
  pwm.on = param.asInt();
}

BLYNK_WRITE(V30) {
  mSpeed.up = param.asInt();
}

BLYNK_WRITE(V31) {
  mSpeed.accel = param.asInt();
}

BLYNK_WRITE(V32) { //Invert direction
  mInvert.set = param.asInt();
}

BLYNK_WRITE(V33) {
  configSaveFlag = param.asInt();
}

BLYNK_WRITE(V34) {
  mSpeed.dn = param.asInt();
}

BLYNK_WRITE(V35) {
 resetFlag = param.asInt(); 
}

BLYNK_WRITE(V36) {
  configLoadFlag = param.asInt();
}

BLYNK_WRITE(V37) {
  ledButtonBrightness = param.asInt();
}
