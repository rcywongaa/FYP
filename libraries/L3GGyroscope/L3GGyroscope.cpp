#include "L3GGyroscope.h"

Gyroscope::Gyroscope():I2CDevice(){
	addr = 0x0F;
}

Gyroscope::Gyroscope(uint8_t sdaPin, uint8_t sclPin):I2CDevice(sdaPin, sclPin){
	addr = 0x0F;
}

void Gyroscope::init(){
	ByteWrite(addr, 0x20, 0xFF);
	ByteWrite(addr, 0x21, 0x00);
	ByteWrite(addr, 0x22, 0x00);
	ByteWrite(addr, 0x23, 0x00);
	ByteWrite(addr, 0x24, 0x00);
}

threeD Gyroscope::getVal(){
	return ByteRead6(addr, 0x28);
}

threeD Gyroscope::ByteRead6(int I2C_Address, int Reg_Address){
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