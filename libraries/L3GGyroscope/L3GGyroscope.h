#ifndef L3GGyroscope_h
#define L3GGyroscope_h

#include "common2.h"
#include "I2CDevice.h"

class Gyroscope : public I2CDevice {
	private:
		threeD ByteRead6(int, int);
		
	public:
		Gyroscope();
		Gyroscope(uint8_t, uint8_t);
		void init();
		threeD getVal();
};

#endif