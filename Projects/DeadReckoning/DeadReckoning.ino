#include <Wire.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"
#include "L3GGyroscope.h"

//Sample Rate:
#define SENSORCOUNT 1
//Digital pins, not analog pins
#define SCL 2
#define SDA1 3
//Default: A4 SDA, A5 SCL
static Compass compass[SENSORCOUNT];
static Accelerometer accel[SENSORCOUNT];
static Gyroscope gyro[SENSORCOUNT];
static long time;
static float dt;
static floatVec3 angles;

void setup(){
  Serial.begin(230400);//Recommended 19200
  Wire.begin();
  for (int i = 0; i < SENSORCOUNT; i++){
    compass[i] = Compass(SDA1 + i, SCL);
    compass[i].init();
    accel[i] = Accelerometer(SDA1 + i, SCL);
    accel[i].init();
    gyro[i] = Gyroscope(SDA1 + i, SCL);
    gyro[i].init();
  }
  angles.x = 0.0;
  angles.y = 0.0;
  angles.z = 0.0;
  time = micros();
}

void loop(){
  if (dt > 0){
    gyro[0].update();
    floatVec3 rot = gyro[0].getRaw();
    angles.x += rot.x * dt;
    angles.y += rot.y * dt;
    angles.z += rot.z * dt;
    sPrintF(angles.x, angles.y, angles.z, true);
  }
  long curr = micros();
  dt = (float)(curr - time) / 1000000.0;
  time = curr;
}
