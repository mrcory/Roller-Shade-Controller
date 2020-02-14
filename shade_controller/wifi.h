


void sendBlynk() { //Blynk Feedback
  Blynk.virtualWrite(V1,stepper.currentPosition());
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
  stepperSpeed = param.asInt();
}

BLYNK_WRITE(V31) { //Acceleration
  stepperAccel = param.asInt();
}

BLYNK_WRITE(V32) { //Invert direction
  invertMotor = param.asInt();
}

BLYNK_WRITE(V33) { //Reset device for inverted setting
  resetFlag = param.asInt();
}
