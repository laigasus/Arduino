int ledPins[]={5, 7, 9, 11, 13};
void setup()
{
   for (int i=0; i<8 ; i++){
       pinMode(ledPins[i], OUTPUT);
   }
}
void loop()
{ 
   oneAfterAnotherNoLoop();
}
void oneAfterAnotherNoLoop(){
   int delayTime=100;
   digitalWrite(ledPins[0],HIGH);
   delay(500);
   digitalWrite(ledPins[1],HIGH);
   delay(500);
   digitalWrite(ledPins[2],HIGH);
   delay(500);
   digitalWrite(ledPins[3],HIGH);
   delay(500);
   digitalWrite(ledPins[4],HIGH);
   delay(500);
   digitalWrite(ledPins[4],LOW);
   delay(500);
}
