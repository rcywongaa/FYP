#include <Wire.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"

#define DISTANCE 0.3
#define NO_COMPASS 5
#define SCL 2
#define SDA1 3
//Default: A4 SDA, A5 SCL
//static Compass compass1 = Compass(3, 2);
static Compass compass[NO_COMPASS];
//static Accelerometer accel = Accelerometer(3, 4);
float pitchInit[NO_COMPASS];
float yawInit[NO_COMPASS];
bool isClockwise;

void setup(){
  Serial.begin(115200);
  Wire.begin();
  for (int i = 0; i < NO_COMPASS; i++){
    compass[i] = Compass(SDA1 + i, SCL);
    compass[i].init();
    threeD rot = compass[i].getVal();
    pitchInit[i] = rot.y;
    yawInit[i] = rot.z;
  }
}

void loop(){
  //int val = accel.getX();
  float x;
  float y;
  for (int i = 0; i < NO_COMPASS; i++){
    threeD rot = compass[i].getVal();
    x = DISTANCE * tan(rot.y - pitchInit[i]);
    y = DISTANCE * tan(rot.z - yawInit[i]);
    if (i == NO_COMPASS-1)
      sPrint(x, y, 0, true);
    //else sPrint(x, y, 0, false);
  }
  //Serial.println(val);
}
