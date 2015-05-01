#include "HMCCompass.h"

#define ALPHA 1 //0.1

/*
+X: forward (away from pins)
+Y: left
+Z: upwards (out of board)
*/

Compass::Compass():I2CDevice(){
	addr = 0x1E;
}

Compass::Compass(uint8_t sdaPin, uint8_t sclPin):I2CDevice(sdaPin, sclPin){
	addr = 0x1E;
}

void Compass::init(){
	ByteWrite(addr, 0x00, 0x18);
	ByteWrite(addr, 0x01, 0x20);
	ByteWrite(addr, 0x02, 0x00);
	prev.x = 0;
	prev.y = 0;
	prev.z = 0;
	delay(50);
}

void Compass::update(){
	raw = ByteRead6(addr, 0x03);
}

floatVec3 Compass::getRaw(){
	return raw;
}

floatVec3 Compass::getFiltered(){
	curr = raw;
	curr.x = (1-ALPHA)*prev.x + ALPHA*curr.x;
	curr.y = (1-ALPHA)*prev.y + ALPHA*curr.y;
	curr.z = (1-ALPHA)*prev.z + ALPHA*curr.z;
	prev = curr;
	return curr;
}

floatVec3 Compass::ByteRead6(int I2C_Address, int Reg_Address){
	floatVec3 result;
	if (isCustom){
		i2c.beginTransmission(I2C_Address);
		i2c.send(byte(Reg_Address));
		i2c.endTransmission();
		i2c.requestFrom(I2C_Address);
		result.x = i2c.receive(1) << 8 | i2c.receive(1);
		result.z = i2c.receive(1) << 8 | i2c.receive(1);
		result.y = i2c.receive(1) << 8 | i2c.receive(0);
		return result;
	}
	else {
		Wire.beginTransmission(I2C_Address);
		Wire.write(byte(Reg_Address));
		Wire.endTransmission();
		Wire.requestFrom(I2C_Address, 6);
		result.x = Wire.read() << 8 | Wire.read();
		result.z = Wire.read() << 8 | Wire.read();
		result.y = Wire.read() << 8 | Wire.read();
		return result;
	}
}
	
/*************** Deprecated ***************/
floatVec3 Compass::getVal(){
	floatVec3 mag = ByteRead6(addr, 0x03);
	mag.x = (1-ALPHA)*prev.x + ALPHA*mag.x;
	mag.y = (1-ALPHA)*prev.y + ALPHA*mag.y;
	mag.z = (1-ALPHA)*prev.z + ALPHA*mag.z;
	prev = mag;

	return mag;
}
/*****************************************/