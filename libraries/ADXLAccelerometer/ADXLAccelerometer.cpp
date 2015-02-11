#include "ADXLAccelerometer.h"

/*
+X: forwards (away from pins)
+Y: right
+Z: downwards (into the board)
*/

#define THRESH 1000
#define ALPHA 0.1

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
	delay(50);
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