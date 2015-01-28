#include "HMCCompass.h"

#define ALPHA 0.1

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

threeD Compass::getVal(){
	threeD mag = ByteRead6(addr, 0x03);
	mag.x = (1-ALPHA)*prev.x + ALPHA*mag.x;
	mag.y = (1-ALPHA)*prev.y + ALPHA*mag.y;
	mag.z = (1-ALPHA)*prev.z + ALPHA*mag.z;
	prev = mag;

	return mag;
}

float Compass::getRoll(){
	return getVal().x;
}

float Compass::getPitch(){
	return getVal().y;
}

float Compass::getYaw(){
	return getVal().z;
}

threeD Compass::ByteRead6(int I2C_Address, int Reg_Address){
	threeD result;
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