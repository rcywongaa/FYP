#include "HMCCompass.h"

#define ALPHA 0.1
/***** Private Methods *****/

/***** Public Methods *****/
Compass::Compass():I2CDevice(){
	addr = 0x1E;
	accel = Accelerometer();
}

Compass::Compass(uint8_t sdaPin, uint8_t sclPin):I2CDevice(sdaPin, sclPin){
	addr = 0x1E;
	accel = Accelerometer(sdaPin, sclPin);
}

void Compass::init(){
	accel.init();
	ByteWrite(addr, 0x00, 0x18);
	ByteWrite(addr, 0x01, 0x20);
	ByteWrite(addr, 0x02, 0x00);
	prev.x = 0;
	prev.y = 0;
	prev.z = 0;
	delay(50);
}

threeD Compass::getVal(){
//http://forum.arduino.cc/index.php/topic,8573.0.html not work
//http://www.timzaman.com/?p=1010 not work
	threeD curr; // x = roll, y = pitch, z = yaw
	threeD acc = accel.getVal();
	threeD mag = ByteRead6(addr, 0x03);
	mag.x = (1-ALPHA)*prev.x + ALPHA*mag.x;
	mag.y = (1-ALPHA)*prev.y + ALPHA*mag.y;
	mag.z = (1-ALPHA)*prev.z + ALPHA*mag.z;
	prev = mag;
//Taken from http://www.freescale.com/files/sensors/doc/app_note/AN4248.pdf
	curr.x = atan2(-acc.y, -acc.z);
	curr.y = atan(-acc.x / ((-acc.y)*sin(curr.x) + (-acc.z)*cos(curr.x)));
	curr.z = atan2((-mag.z*sin(curr.x) - (-mag.y)*cos(curr.x)), (mag.x*cos(curr.y) + (-mag.y)*sin(curr.y)*sin(curr.x) + (-mag.z)*sin(curr.y)*cos(curr.x)));
	//sPrint(curr.x * 180/PI, curr.y * 180/PI, curr.z * 180/PI, true);
	return curr;
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