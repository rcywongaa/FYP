#include "common2.h"

void setup(){
  Serial.begin(19200);
}

void loop(){
  for (float i = -PI; i < PI; i+=0.001){
    sPrintF(0, 0, 0, false);
    sPrintF(0, 0, 0, false);
    sPrintF(i, 0, 0, false);
    sPrintF(0, i, 0, false);
    sPrintF(0, 0, i, false);
    sPrintF(0, 0, 0, false);
    Serial.print("0,0,0,0,0,");
    Serial.println("0,0");
    delay(50);
  }
}
