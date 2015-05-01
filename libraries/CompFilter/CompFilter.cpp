#include "CompFilter.h"

#define DWEIGHT 0.85
CompFilter::CompFilter(){
	prevAngle.x = 0.0;
	prevAngle.y = 0.0;
	prevAngle.z = 0.0;
	prevInc.x = 0.0;
	prevInc.y = 0.0;
	prevInc.z = 0.0;
}

//Check axis directions of calcAngles and gyroscope
floatVec3 CompFilter::evaluate(floatVec3 staticEst, floatVec3 increment, float dt){
	if (dt < 0.0 || (prevAngle.x == 0.0 && prevAngle.y == 0.0 && prevAngle.z == 0.0)){
		prevAngle = staticEst;
		return prevAngle;
	}
	floatVec3 gyrEstimate;
	gyrEstimate.x = prevAngle.x + prevInc.x * dt;
	gyrEstimate.y = prevAngle.y - prevInc.y * dt;
	gyrEstimate.z = prevAngle.z - prevInc.z * dt;
	
	if (abs(gyrEstimate.x - staticEst.x) > 1.5 * PI)
		gyrEstimate.x = gyrEstimate.x + (gyrEstimate.x > staticEst.x ? -2 * PI : 2 * PI);
	if (abs(gyrEstimate.y - staticEst.y) > 1.5 * PI)
		gyrEstimate.y = gyrEstimate.y + (gyrEstimate.y > staticEst.y ? -2 * PI : 2 * PI);
	if (abs(gyrEstimate.z - staticEst.z) > 1.5 * PI)
		gyrEstimate.z = gyrEstimate.z + (gyrEstimate.z > staticEst.z ? -2 * PI : 2 * PI);
		
	floatVec3 result;
	if (staticEst.x == 0.00)
		result.x = 0.00;
	else
		result.x = DWEIGHT * gyrEstimate.x + (1-DWEIGHT) * staticEst.x;
	result.y = DWEIGHT * gyrEstimate.y + (1-DWEIGHT) * staticEst.y;
	result.z = DWEIGHT * gyrEstimate.z + (1-DWEIGHT) * staticEst.z;
	prevInc = increment;
	prevAngle = result;
	return result;
}