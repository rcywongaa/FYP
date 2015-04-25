#ifndef L3GGyroscope_h
#define L3GGyroscope_h

#include "common2.h"
#include "I2CDevice.h"

class Gyroscope : public I2CDevice {
	private:
		floatVec3 ByteRead6(int, int);
		floatVec3 prev;
		floatVec3 curr;
		floatVec3 raw;
		
	public:
		Gyroscope();
		Gyroscope(uint8_t, uint8_t);
		void init();
		void update();
		floatVec3 getRaw();
		floatVec3 getFiltered();
		
		/*********** Deprecated *********/
		floatVec3 getVal();
};

#endif