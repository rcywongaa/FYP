#ifndef HMCCompass_h
#define HMCCompass_h

#include "common2.h"
#include "ADXLAccelerometer.h"
#include "I2CDevice.h"
//#include "Comp6DOF_n0m1.h"

class Compass : public I2CDevice {
	private:
		floatVec3 ByteRead6(int, int);
		floatVec3 prev;
		floatVec3 curr;
		floatVec3 raw;

	public:
		Compass();
		Compass(uint8_t, uint8_t);
		void init();
		void update();
		floatVec3 getRaw();
		floatVec3 getFiltered();
		
		/************** Deprecated *************/
		floatVec3 getVal();
};

#endif