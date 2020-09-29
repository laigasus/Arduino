#include <Servo.h>                        // Include servo library
Servo servoRight;                         // Declare right servo
Servo servoLeft;                          // Declare left servo

 
void setup()                                // Built in initialization block
{
  servoRight.attach(12);                    // Attach right signal to pin 12
  servoLeft.attach(13);                    // Attach right signal to pin 13
  servoRight.writeMicroseconds(1300);      // Right wheel clockwise
  servoLeft.writeMicroseconds(1700);      // Right wheel counterclockwise
  delay(3000);                              // ...for 3 seconds
  servoRight.writeMicroseconds(1500);      // Stay still
  servoLeft.writeMicroseconds(1500);      // Stay still
  delay(1000);                              // ...for 3 seconds
  servoRight.writeMicroseconds(1700);    // Right wheel counterclockwise
  servoLeft.writeMicroseconds(1300);    // Right wheel clockwise
  delay(3000);                               // ...for 3 seconds
  servoRight.writeMicroseconds(1500);    // Stay still
  servoLeft.writeMicroseconds(1500);    // Stay still
}  
void loop()                             // Main loop auto-repeats
{                                       // Empty, nothing needs repeating
}
