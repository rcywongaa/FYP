#include "common2.h"

int BinToInt(int msb, int lsb){
  msb = msb << 8;
  int value = msb;
  value = value | lsb;
  return value;
}

void sPrint(float a, float b, float c, bool newline){
	Serial.print(a);
	Serial.print(",");
	Serial.print(b);
	Serial.print(",");
	Serial.print(c);
	if (newline)
		Serial.println();
	else
		Serial.print(",");
}