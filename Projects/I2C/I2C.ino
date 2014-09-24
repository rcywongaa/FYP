#include <Wire.h>

void ByteWrite(int, int, int);
int ByteRead(int, int);
void LED_On();
void LED_Off();

int ADXL_Address = 0x53;

void setup(){
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  ADXL_Init();
  LED_Off();
}

void loop(){
  int val = 0;
  val = GetZ();
  if (val < 0)
    val = -val;
/*
  LED_On();
  delay(val);
  LED_Off();
  delay(val);
*/
  analogWrite(11, val);
}

void LED_On(){
  digitalWrite(13, HIGH);
}

void LED_Off(){
  digitalWrite(13, LOW);
}

int GetZ(){
  int Z=0;
  Z = BinToInt(ByteRead(ADXL_Address, 0x37), ByteRead(ADXL_Address, 0x36));
  return Z;
}

void ADXL_Init(){
  ByteWrite(ADXL_Address, 0x2C, 0x0F);
  ByteWrite(ADXL_Address, 0x31, 0x01);
  ByteWrite(ADXL_Address, 0x2D, 0x28);
  ByteWrite(ADXL_Address, 0x38, 0x00);
}

void ByteWrite(int I2C_Address, int Reg_Address, int data){
  Wire.beginTransmission(I2C_Address);
  Wire.write(byte(Reg_Address));
  Wire.write(byte(data));
  Wire.endTransmission();
  delay(50);
}

int ByteRead(int I2C_Address, int Reg_Address){
  int data = 0;
  Wire.beginTransmission(I2C_Address);
  Wire.write(byte(Reg_Address));
  Wire.endTransmission();
  Wire.requestFrom(I2C_Address, 1);
  delay(20);
  if (Wire.available() != 0){
    data = Wire.read();
//    LED_On();
  }
  delay(20);
//  if (data != 0)
//    LED_On();
  return data;
}

/*
Input: 8MSB, 8LSB in 2's Complement
Output: Decimal Value (integer)
*/
int BinToInt(int msb, int lsb){
  msb = msb << 8;
  /***** Handles error reading at 0 *****/
  if (msb == -256 && lsb <= 20)
    msb = 0;
  else if (msb == 0 && lsb >= 235)
    msb = -256;
  int value = msb;
  value = value | lsb;
  //value = lsb;
  Serial.println(value);
  return value;
}
