//Code for Nema 17

#define stepperNema

/* Config Section
 *  
 */

/* Set positions 1-4 in steps for stop locations. 
 * Scaled is figured for 10 positions between 1 and 4
 * Position:     1    2     3     4   Scaled
 */
long shade[5] = {0,5,10,20,0};

#define dirPin    1  //Direction control pin
#define stepPin   2  //Step control pin
#define ctrlType  1  //Control type for accelstepper
#define enablePin 11 //Pin to enable/disable motor
#define brakePin  10 //Pin used to control the brake

//Variables
bool brakeOn = true;



//Funtions
void motorOff() {
    if (brakeOn == false) {
      digitalWrite(brakePin,LOW);
      brakeOn = true;
      digitalWrite(enablePin,HIGH);
      delay(100); //Actuation time
    }
}

void brakeCheck() {
  if (currentDistance != 0 && brakeOn == true) {
      digitalWrite(enablePin,LOW);
      digitalWrite(brakePin,HIGH);
      brakeOn = false;
      delay(100); //Actuation time
  }
}
