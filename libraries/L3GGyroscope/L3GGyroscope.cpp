#include "L3GGyroscope.h"

#define ALPHA 1

Gyroscope::Gyroscope():I2CDevice(){
	addr = 0x69;
}

Gyroscope::Gyroscope(uint8_t sdaPin, uint8_t sclPin):I2CDevice(sdaPin, sclPin){
	addr = 0x69;
}

void Gyroscope::init(){
	ByteWrite(addr, 0x20, 0xFF);
	ByteWrite(addr, 0x21, 0x00);
	ByteWrite(addr, 0x22, 0x00);
	ByteWrite(addr, 0x23, 0x10);
	ByteWrite(addr, 0x24, 0x00);
	delay(50);
}

void Gyroscope::update(){
	raw = ByteRead6(addr, 0x28);
}

floatVec3 Gyroscope::getRaw(){
	return raw;
}

floatVec3 Gyroscope::getFiltered(){
	curr = raw;
	curr.x = (1-ALPHA)*prev.x + ALPHA*curr.x;
	curr.y = (1-ALPHA)*prev.y + ALPHA*curr.y;
	curr.z = (1-ALPHA)*prev.z + ALPHA*curr.z;
	prev = curr;
	return curr;
}

floatVec3 Gyroscope::ByteRead6(int I2C_Address, int Reg_Address){
	floatVec3 result;
	if (isCustom){
		i2c.beginTransmission(I2C_Address);
		i2c.send(byte(Reg_Address) | 0x80);
		i2c.endTransmission();
		i2c.requestFrom(I2C_Address);
		result.x = i2c.receive(1) | (i2c.receive(1) << 8);
		result.y = i2c.receive(1) | (i2c.receive(1) << 8);
		result.z = i2c.receive(1) | (i2c.receive(0) << 8);
		i2c.endTransmission();
	}
	else {
		Wire.beginTransmission(I2C_Address);
		Wire.write(byte(Reg_Address));
		Wire.endTransmission();
		Wire.requestFrom(I2C_Address, 6);
		result.x = Wire.read() | (Wire.read() << 8);
		result.y = Wire.read() | (Wire.read() << 8);
		result.z = Wire.read() | (Wire.read() << 8);
	}
	//Convert results to radians per second
	result.x *= 17.5/1000.0;
	result.y *= 17.5/1000.0;
	result.z *= 17.5/1000.0;
	result = degToRad(result);
	return result;
}

/***************** Deprecated *****************/
floatVec3 Gyroscope::getVal(){
	return ByteRead6(addr, 0x28);
}
/**********************************************/