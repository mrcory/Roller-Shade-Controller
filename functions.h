




void configSave() {
  //Position 400 is reserved for the blynk token
  EEPROM.write(0,1); //Flag for autoload
  int i=5;
  EEPROM.put(i,savedPosition);
  i+=sizeof(savedPosition);
  EEPROM.commit();
  Serial.println("Config Saved");
}

void configLoad() {
  int i=5;
  EEPROM.get(5,savedPosition);
  Serial.println("Config Loaded");
  
  if (EEPROM.read(399) == 1) {
    EEPROM.get(400,token);
    Serial.print("Token: ");
    Serial.println(token);
  }

Serial.println(savedPosition);
}
