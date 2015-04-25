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
}

void loop(){
  for (int i = 0; i < SENSORCOUNT; i++){
    floatVec3 acc = accel[i].getVal();
    floatVec3 mag = compass[i].getVal();
    floatVec3 gyr = gyro[i].getVal();
    sPrintF(acc.x, acc.y, acc.z, false);
    sPrintF(mag.x, mag.y, mag.z, false);
    if (i == SENSORCOUNT - 1)
      sPrintF(gyr.x, gyr.y, gyr.z, true);
    else sPrintF(gyr.x, gyr.y, gyr.z, false);
  }
}
