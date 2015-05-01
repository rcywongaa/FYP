#include <Wire.h>
#include <SPI.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"
#include "ADNS9800.h"

#define BAUDRATE 230400 //Recommended 230400
#define SENSORNUMBER 0 //The sensor to monitor (0 - 5)

#define SCL 2
#define SDA1 3
#define ROLLTHRESH1 65 * PI / 180
#define ROLLTHRESH2 25 * PI / 180
#define PITCHTHRESH 75 * PI / 180
//Schmitt Trigger thresholds
//Default: A4 SDA, A5 SCL
static Compass compass;
static Accelerometer accel;
static boolean hasRoll = false;

void setup(){
  Serial.begin(BAUDRATE);
  Wire.begin();
  accel = Accelerometer(SDA1 + SENSORNUMBER, SCL);
  accel.init();
  compass = Compass(SDA1 + SENSORNUMBER, SCL);
  compass.init();
  hasRoll = false;
}

void loop(){
  float x;
  float y;
  
  floatVec3 angle;
  accel.update();
  compass.update();
  floatVec3 acc = accel.getFiltered();
  floatVec3 mag = compass.getFiltered();
  //Use palm angles to determine hasRoll
  if (SENSORNUMBER == 0){
    floatVec3 pRot = calcAngles(acc, mag);
    if (abs(pRot.x) > ROLLTHRESH1 && hasRoll == false && abs(pRot.y) < PITCHTHRESH)
      hasRoll = true;
    if ((abs(pRot.x) < ROLLTHRESH2 && hasRoll == true) || abs(pRot.y) > PITCHTHRESH){
      hasRoll = false;
    }
  }
  if (hasRoll) angle = calcAngles(acc, mag);
  else angle = calcAnglesNoRoll(acc, mag);
  sPrintF(angle.x, angle.y, angle.z, false);
  sPrintF(acc.x, acc.y, acc.z, false);
  sPrintF(mag.x, mag.y, mag.z, true);
}
