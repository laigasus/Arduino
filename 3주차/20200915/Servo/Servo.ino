#include <Servo.h>           // Include servo library
Servo servoLeft;                // Declare left servo signal
Servo servoRight;              // Declare right servo signal
void setup()                     // Built in initialization block
{
  servoLeft.attach(13);         // Attach left signal to pin 13
  servoRight.attach(12);       // Attach left signal to pin 12
  servoLeft.writeMicroseconds(1500);  // 1.5 ms stay still sig, pin13
  servoRight.writeMicroseconds(1500); // 1.5 ms stay still sig, pin12
}
void loop()                        // Main loop auto-repeats
{                                     // Empty, nothing needs repeating
}
