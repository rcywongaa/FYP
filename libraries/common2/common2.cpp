#include "common2.h"

void sPrint(floatVec3 fData[], int fSize, bool bData[], int bSize, sIntVec2 mData){
	for (int i = 0; i < fSize; i++){
		Serial.print(fData[i].x, 5);
		Serial.print(",");
		Serial.print(fData[i].y, 5);
		Serial.print(",");
		Serial.print(fData[i].z, 5);
		Serial.print(",");
	}
	
	for (int i = 0; i < bSize; i++){
		Serial.print(bData[i]);
		Serial.print(",");
	}
	
	Serial.print(mData.x);
	Serial.print(",");
	Serial.print(mData.y);
	Serial.println();
}

void sPrint(sIntVec3 fData[], int fSize, bool bData[], int bSize, sIntVec2 mData){
	for (int i = 0; i < fSize; i++){
		Serial.print(fData[i].x);
		Serial.print(",");
		Serial.print(fData[i].y);
		Serial.print(",");
		Serial.print(fData[i].z);
		Serial.print(",");
	}
	
	for (int i = 0; i < bSize; i++){
		Serial.print(bData[i]);
		Serial.print(",");
	}
	
	Serial.print(mData.x);
	Serial.print(",");
	Serial.print(mData.y);
	Serial.println();
}

void sPrintI(int a, int b, int c, bool newline){
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

void sPrintF(float a, float b, float c, bool newline){
	Serial.print(a, 5);
	Serial.print(",");
	Serial.print(b, 5);
	Serial.print(",");
	Serial.print(c, 5);
	if (newline)
		Serial.println();
	else
		Serial.print(",");
}

floatVec3 calcAngles(floatVec3 acc, floatVec3 mag){
//Taken from http://www.freescale.com/files/sensors/doc/app_note/AN4248.pdf
//http://forum.arduino.cc/index.php/topic,8573.0.html not work
//http://www.timzaman.com/?p=1010 not work
	floatVec3 rot;
	/* Version 1 */
	rot.x = atan2(acc.y, acc.z); //roll
	rot.y = atan(acc.x / ((acc.y)*sin(rot.x) + (acc.z)*cos(rot.x))); //pitch
	rot.z = atan2((-mag.z*sin(rot.x) - (-mag.y)*cos(rot.x)), (mag.x*cos(rot.y) + (-mag.y)*sin(rot.y)*sin(rot.x) + (-mag.z)*sin(rot.y)*cos(rot.x))); //yaw
	return rot;
}

floatVec3 calcAnglesNoRoll(floatVec3 acc, floatVec3 mag){
	/* Version 2 */
	floatVec3 rot;
	rot.x = 0;
	rot.y = atan2(acc.x, ((acc.y)*sin(rot.x) + (acc.z)*cos(rot.x))); //pitch
	rot.z = atan2((-mag.z*sin(rot.x) - (-mag.y)*cos(rot.x)), (mag.x*cos(rot.y) + (-mag.y)*sin(rot.y)*sin(rot.x) + (-mag.z)*sin(rot.y)*cos(rot.x))); //yaw
	return rot;
}

float getEnergySq(floatVec3 acc){
	return acc.x * acc.x + acc.y * acc.y + acc.z + acc.z;
}

floatVec3 degToRad(floatVec3 deg){
	floatVec3 result;
	result.x = deg.x * PI / 180.0;
	result.y = deg.y * PI / 180.0;
	result.z = deg.z * PI / 180.0;
	return result;
}

floatVec3 radToDeg(floatVec3 rad){
	floatVec3 result;
	result.x = rad.x / PI * 180.0;
	result.y = rad.y / PI * 180.0;
	result.z = rad.z / PI * 180.0;
	return result;
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