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
int xDistance
int yDistance
*/

//Default: A4 SDA, A5 SCL

#include <Wire.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"
#include "L3GGyroscope.h"

// SPI and misc pins for the ADNS
#include <SPI.h>
#include <avr/pgmspace.h>
#include "ADNS9800.h"
#include "firmware.h"

#define SENSORCOUNT 6
#define HASMOUSE true

#define BAUDRATE 230400 //Max: 230400
#define SCL 2
#define SDA1 3

//Complementary Filter Constants
#define DWEIGHT 0.9 //Weight of gyroscope

//Schmitt Trigger Constants
#define KEYBOARDMODE 1
#define MOUSEMODE 2
#define ROLLTHRESH1 65 * PI / 180
#define ROLLTHRESH2 25 * PI / 180
#define PITCHTHRESH 75 * PI / 180

//Tap Detection Constants
#define TAPTHRESH 180
#define TIMEOUT 6 //max 1 tap per 0.25 sec
#define WINDOWSIZE 4


static Compass compass[SENSORCOUNT];
static Accelerometer accel[SENSORCOUNT];
static Gyroscope gyroscope[SENSORCOUNT];
static bool hasRoll = false;
static floatVec3 acc;
static floatVec3 mag;
static floatVec3 gyr;
static floatVec3 angles[SENSORCOUNT];
static OpticalSensor optic;
static long time;
static float dt;
static int mode;

//Tap Detection Variables
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
    gyroscope[i] = Gyroscope(SDA1 + i, SCL);
    gyroscope[i].init();
  }
  hasRoll = false;
  
  optic = OpticalSensor();
  optic.init();
  time = millis();
  dt = 0.0;
  mode = MOUSEMODE;
}

void loop(){
  if (Serial.available())
    mode = Serial.read();
  dt = (float)(millis() - time)/1000.0;
  time = millis();
  for (int i = 0; i < SENSORCOUNT; i++){
    accel[i].update();
    compass[i].update();
    acc = accel[i].getFiltered();
    mag = compass[i].getFiltered();
    //Use palm angles to determine hasRoll
    if (i == 0){
      floatVec3 pRot = calcAngles(acc, mag);
      //if (abs(pRot.x) > ROLLTHRESH1 && hasRoll == false && abs(pRot.y) < PITCHTHRESH)
      if (mode == MOUSEMODE && abs(pRot.y) < PITCHTHRESH)
        hasRoll = true;
      //if ((abs(pRot.x) < ROLLTHRESH2 && hasRoll == true) || abs(pRot.y) > PITCHTHRESH)
      if (mode == KEYBOARDMODE || abs(pRot.y) > PITCHTHRESH)
        hasRoll = false;
    }
    floatVec3 sAngles;
    if (hasRoll) angles[i] = calcAngles(acc, mag);
    else angles[i] = calcAnglesNoRoll(acc, mag);
    
    /****************** COMPLEMENTARY FILTER ***************/
    //gyroscope[i].update();
    //gyr = gyroscope[i].getFiltered();
    //angles[i].x = DWEIGHT * (angles[i].x + gyr.x*dt) + (1-DWEIGHT) * (sAngles.x);
    //angles[i].y = DWEIGHT * (angles[i].y + gyr.y*dt) + (1-DWEIGHT) * (sAngles.y);
    //angles[i].z = DWEIGHT * (angles[i].z + gyr.z*dt) + (1-DWEIGHT) * (sAngles.z);
        
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
