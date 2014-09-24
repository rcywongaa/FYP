#ifndef common2_h
#define common2_h

#include "math.h"
#include "Arduino.h"
#include "Wire.h"
#include "SoftI2CMaster.h"

struct threeD {
	float x;
	float y;
	float z;
};
int BinToInt(int, int);
void sPrint(float, float, float, bool);

#endif