


void sendBlynk() { //Blynk Feedback
  Blynk.virtualWrite(V1,stepper.currentPosition());
  Blynk.virtualWrite(V27,pwmBrightness);
  Blynk.virtualWrite(V28,pwmOn);

  if (firstRun == true) {
    Blynk.virtualWrite(V11,shade[4]);
    Blynk.virtualWrite(V26,pulseMax);
    Blynk.virtualWrite(V27,pwmBrightness);
    Blynk.virtualWrite(V30,stepperSpeed[0]);
    Blynk.virtualWrite(V31,stepperAccel);
    Blynk.virtualWrite(V32,invertMotor[1]);
    Blynk.virtualWrite(V34,stepperSpeed[1]);
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
  pwmBrightness = param.asInt();
}

BLYNK_WRITE(V28) {
  pwmOn = param.asInt();
}


//------Configure device via Blynk

BLYNK_WRITE(V30) { //Speed
  stepperSpeed[0] = param.asInt();
}

BLYNK_WRITE(V31) { //Acceleration
  stepperAccel = param.asInt();
}

BLYNK_WRITE(V32) { //Invert direction
  invertMotor[0] = param.asInt();
}

BLYNK_WRITE(V33) { //Reset device for inverted setting
  configSaveFlag = param.asInt();
}

BLYNK_WRITE(V34) {
  stepperSpeed[1] = param.asInt();
}
