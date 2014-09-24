#ifndef I2CDevice_h
#define I2CDevice_h

#include "common2.h"

class I2CDevice {
	public:
		int addr;
		bool isCustom;
		SoftI2CMaster i2c;
		I2CDevice();
		I2CDevice(uint8_t, uint8_t);
		void ByteWrite(int, int, int);
		int ByteRead(int, int);
		//virtual threeD ByteRead6(int, int);
};

#endif