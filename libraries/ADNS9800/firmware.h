// the firmeware that is uploaded in the ADNS each time it boots

#ifndef firmware_h
#define firmware_h

#include <avr/pgmspace.h>

// Firmware "adns9800_srom_A4.txt" from 
// This firmware is Copyright Avago, please refer to them concerning modifications.

extern const unsigned short firmware_length;

extern prog_uchar firmware_data[];

#endif