#ifndef ADXLAccelerometer_h
#define ADXLAccelerometer_h

#include "common2.h"
#include "I2CDevice.h"

class Accelerometer : public I2CDevice{
	private:
		threeD prev;
		threeD ByteRead6(int, int);

	public:
		Accelerometer();
		Accelerometer(uint8_t, uint8_t);
		void init();
		threeD getVal();
};

#endif