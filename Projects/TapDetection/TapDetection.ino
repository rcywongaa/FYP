/*
Two Methods for tap detection:

1. Record previous n entries, tap occurs when max - min > threshold

2. Create moving average, tap occurs when curr - avg > threshold
*/

#include <Wire.h>
#include <SPI.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"
#include "HMCCompass.h"

#define BAUDRATE 14400 //Recommended 14400
#define SENSORCOUNT 6
#define SCL 2
#define SDA1 3
#define FRAMERATE 24
#define ALPHA 0.1
#define TAPTHRESH 125
#define TIMEOUT 6 //max 1 tap per 0.25 sec
#define WINDOWSIZE 4

static Compass compass[SENSORCOUNT];
static Accelerometer accel[SENSORCOUNT];
static floatVec3 acc;
static floatVec3 mag;
static sIntVec3 angles[SENSORCOUNT];
static bool taps[SENSORCOUNT];
static int counter[SENSORCOUNT];

static float window[SENSORCOUNT][WINDOWSIZE];

void setup(){
  Serial.begin(BAUDRATE);
  Wire.begin();
  for (int i = 0; i < SENSORCOUNT; i++){
    accel[i] = Accelerometer(SDA1 + i, SCL);
    accel[i].init();
    compass[i] = Compass(SDA1 + i, SCL);
    compass[i].init();
    
    taps[i] = false;
    counter[i] = 0;
  }
}

void loop(){
  for (int i = 0; i < SENSORCOUNT; i++){
    taps[i] = false;
    int maxVal = 0;
    int minVal = 1e9;
    accel[i].update();
    compass[i].update();
    acc = accel[i].getRaw();
    if (counter[i] < TIMEOUT)
      counter[i]++; 
    for (int j = WINDOWSIZE - 1; j >= 0; j--){
      if (j == 0)
        window[i][j] = sqrt(acc.x * acc.x + acc.z * acc.z);
      else
        window[i][j] = window[i][j-1];
      maxVal = max(window[i][j], maxVal);
      minVal = min(window[i][j], minVal);
    }
    if (maxVal - minVal > TAPTHRESH && counter[i] == TIMEOUT){
      taps[i] = true;
      counter[i] = 0;
    }
  }
  
  //floatVec3 raw = accel[0].getRaw(); sPrint(sqrt(raw.x * raw.x + raw.y * raw.y  + raw.z * raw.z), sqrt(raw.x * raw.x + raw.z * raw.z), raw.y, true); delay(10);
  sPrint(taps, sizeof(taps) / sizeof(bool));
  delay(15);
}
