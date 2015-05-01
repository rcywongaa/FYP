#ifndef ADXLAccelerometer_h
#define ADXLAccelerometer_h

#include "common2.h"
#include "I2CDevice.h"

/*
+X: forwards (away from pins)
+Y: right
+Z: downwards (into the board)
*/

class Accelerometer : public I2CDevice{
	private:
		floatVec3 prev;
		floatVec3 raw;
		floatVec3 curr;
		floatVec3 ByteRead6(int, int);

	public:
		Accelerometer();
		Accelerometer(uint8_t, uint8_t);
		void init();
		void update();
		floatVec3 getFiltered();
		floatVec3 getRaw();
		
		/************ Deprecated ***************/
		floatVec3 getVal();
};

#endif