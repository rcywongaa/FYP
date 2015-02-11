#include <Wire.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"

#define BAUDRATE 14400
#define SENSORCOUNT 5
#define SCL 2
#define SDA1 3
#define ROLLTHRESH1 65 * PI / 180
#define ROLLTHRESH2 25 * PI / 180
#define PITCHTHRESH 75 * PI / 180
//Schmitt Trigger thresholds
//Default: A4 SDA, A5 SCL
static Compass compass[SENSORCOUNT];
static Accelerometer accel[SENSORCOUNT];
static boolean hasRoll = false;

void setup(){
  Serial.begin(14400);//Recommended 19200
  Wire.begin();
  for (int i = 0; i < SENSORCOUNT; i++){
    accel[i] = Accelerometer(SDA1 + i, SCL);
    accel[i].init();
    compass[i] = Compass(SDA1 + i, SCL);
    compass[i].init();
  }
  hasRoll = false;
}

void loop(){
  float x;
  float y;
  for (int i = 0; i < SENSORCOUNT; i++){
    threeD acc = accel[i].getVal();
    threeD mag = compass[i].getVal();
    threeD rot = calcAngles(acc, mag);
    if (abs(rot.x) > ROLLTHRESH1 && hasRoll == false && abs(rot.y) < PITCHTHRESH)
      hasRoll = true;
    if (abs(rot.x) < ROLLTHRESH2 && hasRoll == true){
      hasRoll = false;
    }
    if (!hasRoll)  rot = calcAnglesNoRoll(acc, mag);
    //rot = calcAngles(acc, mag);
    //sPrint(acc.x, acc.y, acc.z, false);
    //sPrint(mag.x, mag.y, mag.z, false);
    //sPrint(rot.x, rot.y, rot.z, true);
    if (i == SENSORCOUNT - 1)
      sPrint(rot.x, rot.y, rot.z, true);
    else sPrint(rot.x, rot.y, rot.z, false);
  }
}
