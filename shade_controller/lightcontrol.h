void lightControl() {
  
  if (lightMode == 1) { //Use FastLED if selected
    if (lightOn == true) {
      fill_solid(leds,NUM_LEDS,CRGB(currentColor[0],currentColor[1],currentColor[2]));
    }
  
    if (lightOn == false && lightOld != lightOn) {
      fill_solid(leds,NUM_LEDS,CRGB(0,0,0));
    }
    lightOld = lightOn;
  
    static bool _direction = false; //Flag for fade direction
  
    if (pulse == true) {
      if (timer(1,pulseSpeed)) {
        if (_direction == true) {
          if (ledBrightness < pulseMax) { ledBrightness+=2; } else {_direction = false;}
        }
  
        if (_direction  == false) {
          if (ledBrightness > 5) { ledBrightness-=2; } else {_direction = true;}
        }    
      }
    }
  }

  if (lightMode == 0) {
    if (pwmOn == false && pwmBrightnessOut > 0) {
      pwmBrightnessOut = 0;
    } else {
      pwmBrightnessOut = pwmBrightness;
    }
  }

//------LED Strip
  
}
