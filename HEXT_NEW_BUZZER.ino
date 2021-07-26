#include "pitches.h"

int PIN_BUZ = D5;
int ArraySize = sizeof(Song_M)/sizeof(int);
                 
void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("*** HexT Buzzer Board Test ***"); 
}

void loop() 
{
  for(int i=0; i<ArraySize; i++)
  {
    tone(PIN_BUZ,Song_M[i]);
    delay(Song_R[i]*200);
    noTone(PIN_BUZ);
  }
  delay(10000);
}
