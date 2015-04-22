#include <SPI.h>
#include <avr/pgmspace.h>
#include "firmware.h"
#include "common2.h"
#include "ADNS9800.h"

#define SCALE 0.0025 //400 = 1cm

static OpticalSensor optic;
static sIntVec2 iDis;

void setup(){
  Serial.begin(14400);
  optic = OpticalSensor();
  optic.init();
  iDis.x = 0;
  iDis.y = 0;
}

void loop(){
  sIntVec2 distance = optic.read();
  iDis.x += distance.x;
  iDis.y += distance.y;
  if (distance.x != 0 && distance.y != 0)
    sPrint((float)iDis.x * SCALE, (float)iDis.y * SCALE, 0, true);
}
