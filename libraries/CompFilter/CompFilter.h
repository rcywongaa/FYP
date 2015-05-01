#ifndef CompFilter_h
#define CompFilter_h

#include "common2.h"

class CompFilter{
	private:
		floatVec3 prevAngle;
		floatVec3 prevInc;
	
	public:
		CompFilter();
		floatVec3 evaluate(floatVec3, floatVec3, float);
};

#endif