#include <Wire.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"
#include "L3GGyroscope.h"
#include "CompFilter.h"

//Sample Rate: 57 at 5 decimal points
//Sample Rate: 86 at 0 decimal points

#define SENSORNUMBER 3 //0 ~ 5
//Digital pins, not analog pins
#define SCL 2
#define SDA1 3
//Default: A4 SDA, A5 SCL
static Compass compass;
static Accelerometer accel;
static Gyroscope gyro;
static CompFilter filter;
static long time;
static float dt;

void setup(){
  Serial.begin(230400);//Recommended 19200
  compass = Compass(SDA1 + SENSORNUMBER, SCL);
  compass.init();
  accel = Accelerometer(SDA1 + SENSORNUMBER, SCL);
  accel.init();
  gyro = Gyroscope(SDA1 + SENSORNUMBER, SCL);
  gyro.init();
  filter = CompFilter();
  time = micros();
  dt = 0.0;
}

void loop(){
  long curr = micros();
  dt = (float)(curr - time)/1000000.0;
  time = curr;
  accel.update();
  compass.update();
  floatVec3 estAngles;
  estAngles = calcAngles(accel.getFiltered(), compass.getFiltered());
  //estAngles = calcAnglesNoRoll(accel[0].getFiltered(), compass[0].getFiltered());
  
  /****************** COMPLEMENTARY FILTER ***************/
  floatVec3 angles;
  gyro.update();
  floatVec3 incAngles = gyro.getRaw();
  angles = filter.evaluate(estAngles, incAngles, dt);
  //sPrintF(angles.x, angles.y, angles.z, false);
  sPrintF(estAngles.x, estAngles.y, estAngles.z, false);
  sPrintF(incAngles.x, incAngles.y, incAngles.z, true);
  delay(50);
}
