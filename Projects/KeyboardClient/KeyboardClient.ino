/*
Returns data in format
float palmAngles * 3
float thumbAngles * 3
float indexAngles * 3
float middleAngles * 3
float ringAngles * 3
float littleAngles * 3
bool thumbTap
bool indexTap
bool middleTap
bool ringTap
bool littleTap
*/

//Default: A4 SDA, A5 SCL

#include <Wire.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"

// SPI and misc pins for the ADNS
#include <SPI.h>
#include <avr/pgmspace.h>
#include "ADNS9800.h"
#include "firmware.h"

#define SENSORCOUNT 6
#define HASMOUSE true

#define BAUDRATE 19200 //Recommended 14400, 19200 for Bluetooth
#define SCL 2
#define SDA1 3
//Schmitt Trigger thresholds
#define ROLLTHRESH1 65 * PI / 180
#define ROLLTHRESH2 25 * PI / 180
#define PITCHTHRESH 75 * PI / 180

//Tap Detection Constants
#define TAPTHRESH 180
#define TIMEOUT 6 //max 1 tap per 0.25 sec
#define WINDOWSIZE 4


static Compass compass[SENSORCOUNT];
static Accelerometer accel[SENSORCOUNT];
static bool hasRoll = false;
static floatVec3 acc;
static floatVec3 mag;
static floatVec3 angles[SENSORCOUNT];
static OpticalSensor optic;

/*** Tap Detection ***/
static bool taps[SENSORCOUNT - 1]; //Ignore palm
static int counter[SENSORCOUNT - 1];
static float window[SENSORCOUNT - 1][WINDOWSIZE];

void setup(){
  Serial.begin(BAUDRATE);
  delay(50);
  for (int i = 0; i < SENSORCOUNT; i++){
    accel[i] = Accelerometer(SDA1 + i, SCL);
    accel[i].init();
    compass[i] = Compass(SDA1 + i, SCL);
    compass[i].init();
  }
  hasRoll = false;
  
  optic = OpticalSensor();
  optic.init();
}

void loop(){
  for (int i = 0; i < SENSORCOUNT; i++){
    accel[i].update();
    compass[i].update();
    acc = accel[i].getFiltered();
    mag = compass[i].getFiltered();
    //Use palm angles to determine hasRoll
    if (i == 0){
      floatVec3 pRot = calcAngles(acc, mag);
      if (abs(pRot.x) > ROLLTHRESH1 && hasRoll == false && abs(pRot.y) < PITCHTHRESH)
        hasRoll = true;
      if ((abs(pRot.x) < ROLLTHRESH2 && hasRoll == true) || abs(pRot.y) > PITCHTHRESH){
        hasRoll = false;
      }
    }
    if (hasRoll) angles[i] = calcAngles(acc, mag);
    else angles[i] = calcAnglesNoRoll(acc, mag);
    
    /**************** TAP DETECTION ********************/
    if (i == 0) //Ignore tap detection for palm
      continue;
    else {
      int k = i-1; //But arrays still start at 0...
      taps[k] = false;
      int maxVal = 0;
      int minVal = 1e9;
      floatVec3 raw = accel[i].getRaw();
      if (counter[k] < TIMEOUT)
        counter[k]++; 
      for (int j = WINDOWSIZE - 1; j >= 0; j--){
        if (j == 0)
          window[k][j] = sqrt(raw.x * raw.x + raw.z * raw.z);
        else
          window[k][j] = window[k][j-1];
        maxVal = max(window[k][j], maxVal);
        minVal = min(window[k][j], minVal);
      }
      if (maxVal - minVal > TAPTHRESH && counter[k] == TIMEOUT){
        taps[k] = true;
        counter[k] = 0;
      }
    }
  }
  
  /************* Mouse Sensor ************/
  sIntVec2 distance;
  if (HASMOUSE){
    distance = optic.read();
  }
  else {
    distance.x = 0;
    distance.y = 0;
  }
  sPrint(angles, sizeof(angles) / sizeof(angles[0]),taps, sizeof(taps) / sizeof(taps[0]), distance); 

}
