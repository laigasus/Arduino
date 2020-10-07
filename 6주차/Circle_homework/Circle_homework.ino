#include <Servo.h>                // Include servo library
Servo servoLeft;                   // Declare left and right servos
Servo servoRight;
byte wLeftOld;                     // Previous loop whisker values
byte wRightOld;
int accel;
void setup()                       // Built-in initialization block
{
  pinMode(7, INPUT);              // Set right whisker pin to input
  pinMode(5, INPUT);              // Set left whisker pin to input
  pinMode(8, OUTPUT);            // Left LED indicator -> output
  pinMode(2, OUTPUT);            // Right LED indicator -> output

  tone(4, 3000, 1000);             // Play tone for 1 second
  delay(1000);                      // Delay to finish tone

  servoLeft.attach(13);              // Attach left signal to pin 13
  servoRight.attach(12);            // Attach right signal to pin 12
  wLeftOld = 0;                       // Init. previous whisker states
  wRightOld = 1;

  accel = 0;
}
void loop()                        // Main loop auto-repeats
{
  byte wLeft = digitalRead(5);       // Copy left result to wLeft
  byte wRight = digitalRead(7);      // Copy right result to wRight

  // Whisker Navigation
  if (wLeft == 0) // If only left whisker contact
  { digitalWrite(8, HIGH);          // Left LED on
    digitalWrite(2, LOW);          // Right LED off

    if (accel == -100) {
      accel -= 0;
    }
    else {
      accel -= 25;
    }
    Move(400, accel);
  }//왼쪽 더듬이 인식시 rightserveo 값 감소=>오른쪽 바퀴속도 증가
  else if (wRight == 0) // If only right whisker contact
  { digitalWrite(8, LOW);          // Left LED off
    digitalWrite(2, HIGH);          // Right LED on
    if (accel == 100) {
      accel += 0;
    }
    else {
      accel += 25;
    }
    Move(400, accel);
  }//오른쪽 더듬이 인식시 rightserveo 값 증가=>오른쪽 바퀴 속도 감소
  else {
    digitalWrite(8, LOW);          // Left LED off
    digitalWrite(2, LOW);          // Right LED off
    Move(400, accel);
  }

}
void Move(int time, int accel)
{
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1400 + accel);
  delay(time); // Maneuver for time ms
}
