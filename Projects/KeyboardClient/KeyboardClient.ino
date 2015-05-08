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
#include "CompFilter.h"

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

//Schmitt Trigger Constants
#define KEYBOARDMODE 1
#define MOUSEMODE 2
#define ROLLTHRESH1 65.0 * PI / 180.0
#define ROLLTHRESH2 25.0 * PI / 180.0
#define PITCHTHRESH 75.0 * PI / 180.0

//Tap Detection Constants
#define TAPTHRESH 150
#define TIMEOUT 6 //max 1 tap per 0.25 sec
#define WINDOWSIZE 4


static Compass compass[SENSORCOUNT];
static Accelerometer accel[SENSORCOUNT];
static Gyroscope gyroscope[SENSORCOUNT];
static bool hasRoll = false;
static floatVec3 angles[SENSORCOUNT];
static OpticalSensor optic;

static CompFilter filter[SENSORCOUNT];
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
    filter[i] = CompFilter();
  }
  hasRoll = true;
  
  optic = OpticalSensor();
  optic.init();
  time = micros();
  dt = 0.0;
  
  mode = MOUSEMODE;
}

void loop(){
  if (Serial.available()) mode = Serial.read();
  
  long curr = micros();
  dt = 0.05; //(float)(curr - time)/1000000.0;
  time = curr;
  
  float tapMax = 0.0;
  int tapIndex = 0;
  
  floatVec3 incAngles;
  
  for (int i = 0; i < SENSORCOUNT; i++){
    accel[i].update();
    compass[i].update();
    floatVec3 acc = accel[i].getFiltered();
    floatVec3 mag = compass[i].getFiltered();
    
    //Use palm angles to determine hasRoll?
    floatVec3 estAngles = calcAngles(acc, mag);
    //if (abs(pRot.x) > ROLLTHRESH1 && hasRoll == false && abs(pRot.y) < PITCHTHRESH)
    if (abs(estAngles.y) > PITCHTHRESH) hasRoll = false;
    else if (mode == MOUSEMODE) hasRoll = true;
    else if (i == 0) hasRoll = true;
    else hasRoll = false;
    //if ((abs(pRot.x) < ROLLTHRESH2 && hasRoll == true) || abs(pRot.y) > PITCHTHRESH)

    if (!hasRoll) estAngles = calcAnglesNoRoll(acc, mag);
    
    /****************** COMPLEMENTARY FILTER ***************/
    gyroscope[i].update();
    incAngles = gyroscope[i].getRaw();
    angles[i] = filter[i].evaluate(estAngles, incAngles, dt);
    //angles[i] = estAngles;
    /**************** TAP DETECTION ********************/
    if (i == 0) //Ignore tap detection for palm
      continue;
    else {
      int k = i-1; //But arrays still start at 0...
      taps[k] = false;
      float maxVal = 0.0;
      float minVal = 1.0e9;
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
      float tapEnergy = maxVal - minVal;
      if (tapEnergy > TAPTHRESH && counter[k] == TIMEOUT){
        if (tapEnergy > tapMax){
          tapMax = tapEnergy;
          tapIndex = k;
        }
      }
    }
  }
  if (tapMax > TAPTHRESH){
    taps[tapIndex] = true;
    for (int i = 0; i < SENSORCOUNT - 1; i++)
      counter[i] = 0;
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
  //sPrintF(incAngles.x, incAngles.y, incAngles.z, true);
}
