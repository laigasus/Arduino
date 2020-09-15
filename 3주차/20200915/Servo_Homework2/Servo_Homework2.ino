#include <Servo.h>           // Include servo library
Servo servo13;                // Declare left servo signal
Servo servo12;              // Declare right servo signal
void setup()                     // Built in initialization block
{
  servo13.attach(13);         // Attach left signal to pin 13
  servo12.attach(12);       // Attach left signal to pin 12
  
  servo13.writeMicroseconds(1300);  // 시계 방향
  servo12.writeMicroseconds(1700);  // 반시계 방향
  delay(3000);                      // 3초 지연
  
  servo13.writeMicroseconds(1500);  // 정지
  servo12.writeMicroseconds(1500);  // 정지
  delay(1000);                      // 1초 지연
  
  servo13.writeMicroseconds(1700);  // 반시계 방향
  servo12.writeMicroseconds(1300);  // 시계 방향
  delay(2000);                      // 2초 지연

  servo13.writeMicroseconds(1500);  // 정지 
  servo12.writeMicroseconds(1500);  // 정지 

}
void loop()                        // Main loop auto-repeats
{                                     // Empty, nothing needs repeating
}
