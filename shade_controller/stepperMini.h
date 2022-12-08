//Code for 28BYJ-48
#define stepperMini


/* Config Section
 *  
 */

/* Set positions 1-4 in steps for stop locations. 
 * Scaled is figured for 10 positions between 1 and 4
 * Position:     1    2     3     4   Scaled
 */
long shade[5] = {0,27333,54666,82000,0};

//Declare motor pins
#define mtrPin1 D5//D5
#define mtrPin2 D7//D7
#define mtrPin3 D6//D6
#define mtrPin4 D8//D8

int stepPerRev = 4096; //Steps needed to make 1 revolution

#define ctrlType 8 //HalfStep


/* Motor Specific Functions
 *  
 */

//Write motor pins low to be sure the motor is unpowered
void motorOff() {
  digitalWrite(mtrPin1,LOW);
  digitalWrite(mtrPin2,LOW);
  digitalWrite(mtrPin3,LOW);
  digitalWrite(mtrPin4,LOW);
  
}
