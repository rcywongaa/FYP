#include <Wire.h>
#include <SPI.h>
#include "common2.h"
#include "SoftI2CMaster.h"
#include "I2CDevice.h"
#include "ADXLAccelerometer.h"

#define SCL 2
#define SDA1 3

//Filter parameters
#define NZEROS 3
#define NPOLES 3
#define GAIN   1.182500985e+01

static float xv[NZEROS+1], yv[NPOLES+1];

static Accelerometer accel;

void setup(){
  Serial.begin(14400);
  Wire.begin();
  accel = Accelerometer(SDA1, SCL);
  accel.init();
}

void loop(){
  float data = accel.getVal().x;
  xv[0] = xv[1]; 
  xv[1] = xv[2]; 
  xv[2] = xv[3]; 
  xv[3] = data / GAIN;
  yv[0] = yv[1]; 
  yv[1] = yv[2]; 
  yv[2] = yv[3]; 
        yv[3] =   (xv[0] + xv[3]) + 3 * (xv[1] + xv[2])
                     + (  0.1978251873 * yv[0]) + ( -0.9103690003 * yv[1])
                     + (  1.4590290622 * yv[2]);
  sPrint(data, yv[3], 0, true);
  delay(61);

}

