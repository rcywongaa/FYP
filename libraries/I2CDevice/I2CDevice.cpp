#include "I2CDevice.h"

#define DELAY 0
#define LONGDELAY 100

I2CDevice::I2CDevice(){
	isCustom = false;
};

I2CDevice::I2CDevice(uint8_t sdaPin, uint8_t sclPin){
	isCustom = true;
	i2c = SoftI2CMaster(sdaPin, sclPin);
};

void I2CDevice::ByteWrite(int I2C_Address, int Reg_Address, int data){
	if (isCustom){
		i2c.beginTransmission(I2C_Address);
		i2c.send(byte(Reg_Address));
		i2c.send(byte(data));
		i2c.endTransmission();
		delay(LONGDELAY);
	}
	else {
		Wire.beginTransmission(I2C_Address);
		Wire.write(byte(Reg_Address));
		Wire.write(byte(data));
		Wire.endTransmission();
		delay(LONGDELAY);
	}
};

int I2CDevice::ByteRead(int I2C_Address, int Reg_Address){
	if (isCustom){
		int data = 5000;
		i2c.beginTransmission(I2C_Address);
		i2c.send(byte(Reg_Address));
		i2c.endTransmission();
		i2c.requestFrom(I2C_Address);
		data = i2c.receive(0);
		delay(DELAY);
		return data;
	}
	else {
		int data = 5000;
		Wire.beginTransmission(I2C_Address);
		Wire.write(byte(Reg_Address));
		Wire.endTransmission();
		Wire.requestFrom(I2C_Address, 1);
		if (Wire.available() != 0){
			data = Wire.read();
		}
		delay(DELAY);
		return data;
	}
};