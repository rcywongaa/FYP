//Downloaded from https://github.com/joshuajnoble/ArduinoADNS-9800/blob/master/OpticalSensor.ino

#include "ADNS9800.h"

// default SPI and misc pins on arduino
#define PIN_SCLK   13
#define PIN_MISO   12
#define PIN_MOSI   11
#define PIN_NCS    10
//#define PIN_MOTION 9
#define DEBUG 0
#define DELAY_SCALE 1

//--------------------------------------------------------------
//                          ADNS
//--------------------------------------------------------------

// Registers
#define REG_Product_ID                           0x00
#define REG_Revision_ID                          0x01
#define REG_Motion                               0x02
#define REG_Delta_X_L                            0x03
#define REG_Delta_X_H                            0x04
#define REG_Delta_Y_L                            0x05
#define REG_Delta_Y_H                            0x06
#define REG_SQUAL                                0x07
#define REG_Pixel_Sum                            0x08
#define REG_Maximum_Pixel                        0x09
#define REG_Minimum_Pixel                        0x0a
#define REG_Shutter_Lower                        0x0b
#define REG_Shutter_Upper                        0x0c
#define REG_Frame_Period_Lower                   0x0d
#define REG_Frame_Period_Upper                   0x0e
#define REG_Configuration_I                      0x0f
#define REG_Configuration_II                     0x10
#define REG_Frame_Capture                        0x12
#define REG_SROM_Enable                          0x13
#define REG_Run_Downshift                        0x14
#define REG_Rest1_Rate                           0x15
#define REG_Rest1_Downshift                      0x16
#define REG_Rest2_Rate                           0x17
#define REG_Rest2_Downshift                      0x18
#define REG_Rest3_Rate                           0x19
#define REG_Frame_Period_Max_Bound_Lower         0x1a
#define REG_Frame_Period_Max_Bound_Upper         0x1b
#define REG_Frame_Period_Min_Bound_Lower         0x1c
#define REG_Frame_Period_Min_Bound_Upper         0x1d
#define REG_Shutter_Max_Bound_Lower              0x1e
#define REG_Shutter_Max_Bound_Upper              0x1f
#define REG_LASER_CTRL0                          0x20
#define REG_Observation                          0x24
#define REG_Data_Out_Lower                       0x25
#define REG_Data_Out_Upper                       0x26
#define REG_SROM_ID                              0x2a
#define REG_Lift_Detection_Thr                   0x2e
#define REG_Configuration_V                      0x2f
#define REG_Configuration_IV                     0x39
#define REG_Power_Up_Reset                       0x3a
#define REG_Shutdown                             0x3b
#define REG_Inverse_Product_ID                   0x3f
#define REG_Motion_Burst                         0x50
#define REG_SROM_Load_Burst                      0x62
#define REG_Pixel_Burst                          0x64

OpticalSensor::OpticalSensor(){
	misoPin = PIN_MISO;
	mosiPin = PIN_MOSI;
	ncsPin = PIN_NCS;
	sclkPin = PIN_SCLK;
}

OpticalSensor::OpticalSensor(uint8_t miso, uint8_t mosi, uint8_t ncs, uint8_t sclk){
	misoPin = miso;
	mosiPin = mosi;
	ncsPin = ncs;
	sclkPin = sclk;
}

void OpticalSensor::init(){
	if (DEBUG)
		Serial.println("Initialization started...");
	SPI.end();
	//pinMode(ncsPin, OUTPUT);
	pinMode(misoPin, INPUT);
	SPI.begin();
	SPI.setBitOrder(MSBFIRST); // transimission of the bits : b7 b6 b5 ... b0 
	SPI.setDataMode(SPI_MODE3); // sampling on rising edge(CPHA=1), and leading edge is setup(=set the value) (CPOL=1)
	SPI.setClockDivider(SPI_CLOCK_DIV8); // 16MHz/8 = 2MHz = the max clock speed of the ADNS, and it works great
	delay(100);
	
	uint8_t laser_ctrl0;
	/*
	//Shutdown sequence
	adns_com_end();
	adns_com_begin();
	adns_write_reg(REG_Power_Up_Reset, 0x5A);
	delay(50);
	adns_write_reg(REG_Observation, 0x00);
	delay(100);
	Serial.print("Observation Reg - ");
	Serial.println(adns_read_reg(REG_Observation), BIN);
	adns_read_reg(REG_Motion);
	adns_read_reg(REG_Delta_X_L);
	adns_read_reg(REG_Delta_X_H);
	adns_read_reg(REG_Delta_Y_L);
	adns_read_reg(REG_Delta_Y_H);
	laser_ctrl0 = adns_read_reg(REG_LASER_CTRL0);
	adns_write_reg(REG_LASER_CTRL0, laser_ctrl0 & 0xfe );
	*/
	//Power up sequence
	adns_com_end(); // ensure that the serial port is reset
	adns_com_begin(); // ensure that the serial port is reset
	//adns_com_end(); // ensure that the serial port is reset
	adns_write_reg(REG_Power_Up_Reset, 0x5A); // force reset
	delay(50); // wait for it to reboot
	// read registers 0x02 to 0x06 (and discard the data)
	adns_read_reg(REG_Motion);
	adns_read_reg(REG_Delta_X_L);
	adns_read_reg(REG_Delta_X_H);
	adns_read_reg(REG_Delta_Y_L);
	adns_read_reg(REG_Delta_Y_H);
	
	delay(100);
	// upload the firmware
	adns_upload_firmware();
	delay(100);
	
	//enable laser(bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
	// reading the actual value of the register is important because the real
	// default value is different from what is said in the datasheet, and if you
	// change the reserved bytes (like by writing 0x00...) it would not work.
	laser_ctrl0 = adns_read_reg(REG_LASER_CTRL0);
	adns_write_reg(REG_LASER_CTRL0, laser_ctrl0 & 0xfe );
	
	delay(100);
	
	if (DEBUG){
		//Test SPI connection
		uint8_t oreg[] = {
		0x00, 0x00, 0x3F, 0x3F, 0x2A, REG_LASER_CTRL0  };
		char* oregname[] = {
		"Product_ID", 
		"Product_ID2", 
		"Inverse_Product_ID", 
		"Inverse_Product_ID2",
		"SROM_ID", 
		"Laser_Ctrl"  };
		uint8_t regres;

		int rctr=0;
		for(rctr=0; rctr<6; rctr++){
			Serial.println("---");
			Serial.println(oregname[rctr]);
			Serial.println(oreg[rctr],HEX);
			regres = adns_read_reg(oreg[rctr]);
			Serial.println(regres,BIN);  
			Serial.println(regres,HEX);  
		}
		delay(100); 
	}
}

sIntVec2 OpticalSensor::read(){
	uint8_t motion = adns_read_reg(REG_Motion);
	sIntVec2 data;
	data.x = (int) adns_read_reg(REG_Delta_X_L) | ((int) adns_read_reg(REG_Delta_X_H)<<8);
	data.y = (int) adns_read_reg(REG_Delta_Y_L) | ((int) adns_read_reg(REG_Delta_Y_H)<<8);
	return data;
}

/*********************************************************************************/

void OpticalSensor::adns_com_begin(){
	digitalWrite(PIN_NCS, LOW);
	delayMicroseconds(1 * DELAY_SCALE);

}

void OpticalSensor::adns_com_end(){
	digitalWrite(PIN_NCS, HIGH);
	delayMicroseconds(1 * DELAY_SCALE);
}


uint8_t OpticalSensor::adns_read_reg(uint8_t reg_addr){
	adns_com_begin();

	// send adress of the register, with MSBit = 0 to indicate it's a read
	SPI.transfer(reg_addr & 0x7f );
	delayMicroseconds(100 * DELAY_SCALE); // tSRAD
	// read data
	uint8_t data = SPI.transfer(0);
	delayMicroseconds(1 * DELAY_SCALE); // tSCLK-NCS for read operation is 120ns
	adns_com_end();
	delayMicroseconds(19 * DELAY_SCALE); //  tSRW/tSRR (=20us) minus tSCLK-NCS

	return data;
}

void OpticalSensor::adns_write_reg(uint8_t reg_addr, uint8_t data){
	adns_com_begin();

	//send adress of the register, with MSBit = 1 to indicate it's a write
	SPI.transfer(reg_addr | 0x80 );
	//sent data
	SPI.transfer(data);

	delayMicroseconds(20 * DELAY_SCALE); // tSCLK-NCS for write operation
	adns_com_end();
	delayMicroseconds(100 * DELAY_SCALE); // tSWW/tSWR (=120us) minus tSCLK-NCS. Could be shortened, but is looks like a safe lower bound 
}

void OpticalSensor::adns_upload_firmware(){
	// send the firmware to the chip, cf p.18 of the datasheet
	if (DEBUG) Serial.println("Uploading firmware...");
	// set the configuration_IV register in 3k firmware mode
	adns_write_reg(REG_Configuration_IV, 0x02); // bit 1 = 1 for 3k mode, other bits are reserved 

	// write 0x1d in SROM_enable reg for initializing
	adns_write_reg(REG_SROM_Enable, 0x1d); 

	// wait for more than one frame period
	delay(100); // assume that the frame rate is as low as 100fps... even if it should never be that low

	// write 0x18 to SROM_enable to start SROM download
	adns_write_reg(REG_SROM_Enable, 0x18); 
	// write the SROM file (=firmware data) 
	adns_com_begin();
	SPI.transfer(REG_SROM_Load_Burst | 0x80); // write burst destination adress
//	SPI.transfer(REG_SROM_Load_Burst); // write burst destination adress
	delayMicroseconds(15 * DELAY_SCALE);

	// send all bytes of the firmware
	unsigned char c;
	for(unsigned short i = 0; i < firmware_length; i++){ 
		c = (unsigned char)pgm_read_byte(firmware_data + i);
//		SPI.transfer(REG_SROM_Load_Burst | 0x80);
		SPI.transfer(c);
		delayMicroseconds(15 * DELAY_SCALE);
	}
	adns_com_end();
	
	if (DEBUG){
		Serial.print("Last byte: ");
		Serial.println((unsigned char)pgm_read_byte(firmware_data + firmware_length - 1));
	}
	
	delayMicroseconds(160 * DELAY_SCALE);
	
	if (DEBUG){
		// read SROM_ID
		uint8_t srom_id = adns_read_reg(REG_SROM_ID);
		Serial.print("SROM ID = ");
		Serial.println(srom_id);
		
		// compare SROM_ID to what is expected
		if(srom_id != 0){
			Serial.println("Firmware uploaded.");
		} else {
			Serial.println("Firmware upload FAILED ><");
		}
	}
}