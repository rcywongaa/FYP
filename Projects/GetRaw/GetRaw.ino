#include <Wire.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"
#include "L3GGyroscope.h"

#define SENSORCOUNT 3
//Digital pins, not analog pins
#define SCL 2
#define SDA1 3
//Default: A4 SDA, A5 SCL
static Compass compass[SENSORCOUNT];
static Accelerometer accel[SENSORCOUNT];
static Gyroscope gyro[SENSORCOUNT];

void setup(){
  Serial.begin(14400);//Recommended 19200
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
    threeD acc = accel[i].getVal();
    threeD mag = compass[i].getVal();
    threeD gyr = gyro[i].getVal();
    sPrint(acc.x, acc.y, acc.z, false);
    sPrint(mag.x, mag.y, mag.z, false);
    if (i == SENSORCOUNT - 1)
      sPrint(gyr.x, gyr.y, gyr.z, true);
    else sPrint(gyr.x, gyr.y, gyr.z, false);
  }
}
