#ifndef HMCCompass_h
#define HMCCompass_h

#include "common2.h"
#include "ADXLAccelerometer.h"
#include "I2CDevice.h"
//#include "Comp6DOF_n0m1.h"

class Compass : public I2CDevice {
	private:
		Accelerometer accel;
		threeD ByteRead6(int, int);
		threeD prev;

	public:
		Compass();
		Compass(uint8_t, uint8_t);
		void init();
		threeD getVal();
		float getRoll();
		float getPitch();
		float getYaw();
};

#endif