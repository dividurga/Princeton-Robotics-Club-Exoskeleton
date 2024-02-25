#include <Stepper.h>
// we will change this to the number of steps on our motor
#define STEPS 100  
// an instance of the stepper class, with the number of steps of the motor and the pins it's attached to
Stepper stepper(STEPS, 8, 9, 10, 11);
int previous = 0;
void setup() {

  Serial.begin(115200);
  // set the speed of the motor
 stepper.setSpeed(30);

}



void loop() {
  int sensorValue = analogRead(A0);  // read the input on analog pin A0

  // to avoid overloading the serial terminal
  /// move a number of steps equal to the change in the
  // sensor reading
  stepper.step(val - previous);

  // remember the previous value of the sensor
  previous = val;
}
