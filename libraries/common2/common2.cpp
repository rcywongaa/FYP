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

threeD calcAngles(threeD acc, threeD mag){
//Taken from http://www.freescale.com/files/sensors/doc/app_note/AN4248.pdf
//http://forum.arduino.cc/index.php/topic,8573.0.html not work
//http://www.timzaman.com/?p=1010 not work
	threeD rot;
	/* Version 1 */
	rot.x = atan2(acc.y, acc.z); //roll
	rot.y = atan(acc.x / ((acc.y)*sin(rot.x) + (acc.z)*cos(rot.x))); //pitch
	rot.z = atan2((-mag.z*sin(rot.x) - (-mag.y)*cos(rot.x)), (mag.x*cos(rot.y) + (-mag.y)*sin(rot.y)*sin(rot.x) + (-mag.z)*sin(rot.y)*cos(rot.x))); //yaw
	return rot;
}

threeD calcAnglesNoRoll(threeD acc, threeD mag){
	/* Version 2 */
	threeD rot;
	rot.x = 0;
	rot.y = atan2(acc.x, ((acc.y)*sin(rot.x) + (acc.z)*cos(rot.x))); //pitch
	rot.z = atan2((-mag.z*sin(rot.x) - (-mag.y)*cos(rot.x)), (mag.x*cos(rot.y) + (-mag.y)*sin(rot.y)*sin(rot.x) + (-mag.z)*sin(rot.y)*cos(rot.x))); //yaw
	return rot;
}

int MedianOfFive(int a, int b, int c, int d, int e)
{
    return b < a ? d < c ? b < d ? a < e ? a < d ? e < d ? e : d
                                                 : c < a ? c : a
                                         : e < d ? a < d ? a : d
                                                 : c < e ? c : e
                                 : c < e ? b < c ? a < c ? a : c
                                                 : e < b ? e : b
                                         : b < e ? a < e ? a : e
                                                 : c < b ? c : b
                         : b < c ? a < e ? a < c ? e < c ? e : c
                                                 : d < a ? d : a
                                         : e < c ? a < c ? a : c
                                                 : d < e ? d : e
                                 : d < e ? b < d ? a < d ? a : d
                                                 : e < b ? e : b
                                         : b < e ? a < e ? a : e
                                                 : d < b ? d : b
                 : d < c ? a < d ? b < e ? b < d ? e < d ? e : d
                                                 : c < b ? c : b
                                         : e < d ? b < d ? b : d
                                                 : c < e ? c : e
                                 : c < e ? a < c ? b < c ? b : c
                                                 : e < a ? e : a
                                         : a < e ? b < e ? b : e
                                                 : c < a ? c : a
                         : a < c ? b < e ? b < c ? e < c ? e : c
                                                 : d < b ? d : b
                                         : e < c ? b < c ? b : c
                                                 : d < e ? d : e
                                 : d < e ? a < d ? b < d ? b : d
                                                 : e < a ? e : a
                                         : a < e ? b < e ? b : e
                                                 : d < a ? d : a;
}