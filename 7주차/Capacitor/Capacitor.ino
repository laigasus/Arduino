#define PRINTVALUE(a) Serial.print(#a);\
                      Serial.print(" = ");\
                      Serial.print(a);\
                      Serial.println(" us");\


void setup()                // Built-in initialization block
{
  tone(4, 3000, 1000);      // Play tone for 1 second
  delay(1000);              // Delay to finish tone
  Serial.begin(9600);      // Set data rate to 9600 bps
}
void loop()                    // Main loop auto-repeats
{ long tLeft = rcTime(8);
long tRight = rcTime(6);
  /*Serial.print("tLeft = ");   // Display tLeft label
  Serial.print(tLeft);        // Display tLeft value
  Serial.println(" us");      // Display tLeft units + newline*/
  PRINTVALUE(tLeft);
  PRINTVALUE(tRight);
  delay(1000);                 // 1 second delay
}                              // rcTime function at pin  
long rcTime(int pin)           // ..returns decay time
{ pinMode(pin, OUTPUT); // Step 1, part 1
  digitalWrite(pin, HIGH); // Step 1, part 2
  delay(1); // Step 2
  pinMode(pin, INPUT); // Step 3 part 1
  digitalWrite(pin, LOW); // Step 3, part 2
  long time = micros(); // Step 4
  while (digitalRead(pin)); // Step 5
  time = micros() - time; // Step 6 & 7
  return time;
}
