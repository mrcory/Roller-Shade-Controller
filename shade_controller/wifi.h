


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

