




void configSave() {
  //Position 400 is reserved for the blynk token
  EEPROM.write(0,1); //Flag for autoload
  int i=5;
  EEPROM.put(i,savedPosition);
  i+=sizeof(savedPosition);
  EEPROM.put(i,lastPosition);
  i+=sizeof(lastPosition); 
  EEPROM.commit();
  Serial.println("Config Saved");
}

void configLoad() {
  int i=5;
  EEPROM.get(5,savedPosition);
  i+=sizeof(savedPosition);
  EEPROM.get(i,lastPosition);
  i+=sizeof(lastPosition);
  Serial.println("Config Loaded");
Serial.println(savedPosition);
}

void ledFeedback() {

  if (ledMode == 0) {
    if (ledTimer > millis() - 250) {
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
}

void ledTurn(int _mode) {
    ledTimer  = millis();
    ledOn = true;
    ledMode = _mode;
}


