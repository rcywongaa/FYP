/*
Downloaded from https://github.com/joshuajnoble/ArduinoADNS-9800/blob/master/ADNS9800.ino
+X: Right
+Y: Down
Scale: 400 = 1cm
*/

#ifndef ADNS9800_h
#define ADNS9800_h

#include <SPI.h>
#include <avr/pgmspace.h>
#include "firmware.h"
#include "common2.h"

//extern const unsigned short firmware_length;
//extern prog_uchar firmware_data[];

class OpticalSensor {
	private:
		uint8_t misoPin;
		uint8_t mosiPin;
		uint8_t ncsPin;
		uint8_t sclkPin;
	public:
		OpticalSensor();
		OpticalSensor(uint8_t, uint8_t, uint8_t, uint8_t);
		void init();
		void adns_com_begin();
		void adns_com_end();
		uint8_t adns_read_reg(byte reg_addr);
		void adns_write_reg(byte reg_addr, byte data);
		void adns_upload_firmware();
		sIntVec2 read();
};

#endif