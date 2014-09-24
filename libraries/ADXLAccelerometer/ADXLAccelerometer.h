#ifndef ADXLAccelerometer_h
#define ADXLAccelerometer_h

#include "common2.h"
#include "I2CDevice.h"

class Accelerometer : public I2CDevice{
	private:
		int MedianOfFive(int, int, int, int, int);
		threeD prev;
		//int BinToInt(int, int);

	public:
		threeD ByteRead6(int, int);
		Accelerometer();
		Accelerometer(uint8_t, uint8_t);
		void init();
		threeD getVal();
		int getX();
		int getY();
		int getZ();
};

#endif