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
      if (timerFunc(pulseSpeed)) {
        if (_direction == true) {
          if (ledBrightness < pulseMax) { ledBrightness+=2; } else {_direction = false;}
        }
  
        if (_direction  == false) {
          if (ledBrightness > 5) { ledBrightness-=2; } else {_direction = true;}
        }    
      }
    }
  }

//PWM Code ---------------------------

  if (lightMode == 0) {
    if (pwm.on == false || pwm.set == 0) {
      pwm.out = 0;
      analogWrite(PWM_PIN,pwm.out);
    } 
  }

  if (pwm.on == true && pwm.set != pwm.out) {
    pwm.out = pwm.set;
    analogWrite(PWM_PIN,pwm.out);
  }

  if (pwm.on == false && pwm.set != pwm.old) {
    pwm.on = true;
  }

  if (pwm.on == true && pwm.set == 0) {
    pwm.on = false;
  }
  
}
