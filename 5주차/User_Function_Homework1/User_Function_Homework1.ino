#include <Servo.h> // Include servo library
Servo servoLeft; // Declare left and right servos
Servo servoRight;
void setup() // Built-in initialization block
{
  tone(4, 3000, 1000); // Play tone for 1 second
  delay(1000); // Delay to finish tone
  servoLeft.attach(13); // Attach left signal to pin 13
  servoRight.attach(12); // Attach right signal to pin 12
  maneuver(200, 200, 2000); // Forward 2 seconds
  maneuver(-200, -200, 1700); // Backward 1.7 seconds
  //장치가 정지하면서 이동하는 거리를 고려하여 1.7초로 정하였음
  maneuver(0, 0, -1); // Disable servos
}
void loop() // Main loop auto-repeats
{ // Empty, nothing needs repeating
}
void maneuver(int speedLeft, int speedRight, int msTime)
{ // speedLeft, speedRight ranges: Backward Linear Stop Linear Forward
  // -200 -100......0......100 200
  servoLeft.writeMicroseconds(1500 + speedLeft); // Set Left servo speed
  servoRight.writeMicroseconds(1500 - speedRight); //Set right servo speed
  if (msTime == -1) // if msTime = -1
  {
    servoLeft.detach(); // Stop servo signals
    servoRight.detach();
  }
  delay(msTime); // Delay for msTime
}
