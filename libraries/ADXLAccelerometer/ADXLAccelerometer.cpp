#include "ADXLAccelerometer.h"

#define THRESH 500
#define ALPHA 0.1
/***** Private Methods *****/

/***** Public Methods *****/
Accelerometer::Accelerometer():I2CDevice(){
	addr = 0x53;
}

Accelerometer::Accelerometer(uint8_t sdaPin, uint8_t sclPin):I2CDevice(sdaPin, sclPin) {
	addr = 0x53;
}

void Accelerometer::init(){
	ByteWrite(addr, 0x2C, 0x0F);
	ByteWrite(addr, 0x31, 0x00);
	ByteWrite(addr, 0x2D, 0x28);
	ByteWrite(addr, 0x38, 0x80);
	prev.x = 0;
	prev.y = 0;
	prev.z = 0;
}

threeD Accelerometer::getVal(){
	threeD curr = ByteRead6(addr, 0x32);
	if (curr.x > THRESH || curr.x < -THRESH)
		curr.x = prev.x;
	if (curr.y > THRESH || curr.y < -THRESH)
		curr.y = prev.y;
	if (curr.z > THRESH || curr.y < -THRESH)
		curr.z = prev.z;
	curr.x = (1-ALPHA)*prev.x + ALPHA*curr.x;
	curr.y = (1-ALPHA)*prev.y + ALPHA*curr.y;
	curr.z = (1-ALPHA)*prev.z + ALPHA*curr.z;
	prev = curr;
	//sPrint(curr.x, curr.y, curr.z, true);
	return curr;
}

int Accelerometer::getX(){
	threeD curr = getVal();
	return curr.x;
}

int Accelerometer::getY(){
	threeD curr = getVal();
	return curr.y;
}

int Accelerometer::getZ(){
	threeD curr = getVal();
	return curr.z;
}

/*
int Accelerometer::BinToInt(int msb, int lsb){
  msb = msb << 8;
  //Handles error reading at 0
  if (msb == -256 && lsb <= 20)
    msb = 0;
  else if (msb == 0 && lsb >= 220)
    msb = -256;
  int value = msb;
  value = value | lsb;
  //sPrint(msb, lsb, value);
  return value;
}
*/

threeD Accelerometer::ByteRead6(int I2C_Address, int Reg_Address){
	threeD result;
	if (isCustom){
		i2c.beginTransmission(I2C_Address);
		i2c.send(byte(Reg_Address));
		i2c.endTransmission();
		i2c.requestFrom(I2C_Address);
		result.x = i2c.receive(1) | (i2c.receive(1) << 8);
		result.y = i2c.receive(1) | (i2c.receive(1) << 8);
		result.z = i2c.receive(1) | (i2c.receive(0) << 8);
		return result;
	}
	else {
		Wire.beginTransmission(I2C_Address);
		Wire.write(byte(Reg_Address));
		Wire.endTransmission();
		Wire.requestFrom(I2C_Address, 6);
		result.x = Wire.read() | (Wire.read() << 8);
		result.y = Wire.read() | (Wire.read() << 8);
		result.z = Wire.read() | (Wire.read() << 8);
		return result;
	}
}

int Accelerometer::MedianOfFive(int a, int b, int c, int d, int e)
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