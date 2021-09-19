/***************************************************************************************************************************/
/*
    A simple library that reads an ADS1115 A/D converter connected via I2C.
    Auther:   David Mottram
    Updated:  2nd April 2020
*/
/***************************************************************************************************************************/

#include "Arduino.h"
#include "Power_ADS1115.h"

// https://www.arduino.cc/en/Reference/Wire
#include <Wire.h>


// constructor
// number of switches, the A/D value for the top switch in the array and the switch debounce period
ADS1115::ADS1115(int address) {

	Wire.begin();         // begin I2C
	_ADS_Address = address;

} // end of function


// test I2C port, return error code. 0 = ADS1115 device present
byte ADS1115::test_AD(void) {

  /*
    error codes
    0:success
    1:data too long to fit in transmit buffer
    2:received NACK on transmit of address
    3:received NACK on transmit of data
    4:other error
  */

  byte error;

  Wire.beginTransmission(_ADS_Address);
  error = Wire.endTransmission();

  return error;

} // end of function


/*
  multiplex selection and restart A/D conversion. A small delay is added to the end of this function
  to allow the A/D data buffer to flush out the last sample and have a new one ready to read.
*/
void ADS1115::set_mux(int mux_channel) {

  // range check, force channel 0 if out of range
  if (mux_channel > 3 || mux_channel < 0) {
    mux_channel = 0;
  }

  // mux selection, makeup the 16 bit config byte
  uint16_t config_reg = 0x8000;
  uint8_t config_low = 0; uint8_t config_high = 0;

  // select mux and continuous conversion
  switch (mux_channel) {
    case (0):
      config_reg = config_reg | AIN0 | Voltage_Range | Op_Mode_Cont | Data_Rate | Comp_Mode;
      break;
    case (1):
      config_reg = config_reg | AIN1 | Voltage_Range | Op_Mode_Cont | Data_Rate | Comp_Mode;
      break;
    case (2):
      config_reg = config_reg | AIN2 | Voltage_Range | Op_Mode_Cont | Data_Rate | Comp_Mode;
      break;
    case (3):
      config_reg = config_reg | AIN3 | Voltage_Range | Op_Mode_Cont | Data_Rate | Comp_Mode;
      break;
  } //end selection

  // make the 16bit config value into two 8bit nibbles
  config_low = config_reg & 0xff; config_high = (config_reg >> 8);

  // setup ADS1115
  Wire.beginTransmission(_ADS_Address);       // I2C address
  Wire.write(Config_Reg);                     // address pointer
  Wire.write(config_high);                    // config high nibble
  Wire.write(config_low);                     // config low nibble
  Wire.endTransmission();                     // end transmission

  // delay to allow the A/D buffer to clear the old value and the new mux input to sample
  // with 860 samples per second the minimum would be 1.16mS, at least twice this value.
  delayMicroseconds(2500);

} // end of function


/*
  read the A/D value from the current mux selected input.
  with the conversion mode set to continuous and the samples per second set to 860 the conversion
  time is around 1.16mS per sample. So there is no point sampling the A/D data buffer more frequently than at least 1.16mS.
  Reading the A/D data buffer via I2C takes around 0.65mS so adding a small delay to the end of this function
  prevents reading the buffer too quickly.
*/
uint16_t ADS1115::read_AD(void) {

  // variables used
  byte Result_MSD;
  byte Result_LSD;
  unsigned int Value = 0;

  // set the address pointer to the data register
  Wire.beginTransmission(_ADS_Address);  // DAC
  Wire.write(Data_Reg);  // pointer address
  Wire.endTransmission();
  // read two bytes of data from above address
  Wire.requestFrom(_ADS_Address, 2); // request 2 bytes from address
  Result_MSD = Wire.read();  // read and store byte
  Result_LSD = Wire.read();  // read and store byte
  Wire.endTransmission();  // end I2C transmission

  // convert display results
  Value = Result_MSD << 8 | Result_LSD;

  // delay to makeup the min read time
  delayMicroseconds(600);

  return (Value);

} // end of function


/*
  take several samples from a selected mux input. Then return the difference between the min and max samples
  providing a peak voltage sample.
*/
uint16_t ADS1115::power_sample(int mux_channel, int samples) {

  // set the mux channel, takes a few mS
  set_mux(mux_channel);

  uint16_t Max = 0x0000;
  uint16_t Min = 0xFFFF;
  uint16_t Sample = 0;

  // take many samples to find the max and min values
  for (int zz = 0; zz < samples; zz++) {

    Sample = read_AD();

    // find new min-max samples
    if (Sample > Max) {
      Max = Sample;
    }
    if (Sample < Min) {
      Min = Sample;
    }

  } // end sample loop

  // over range check, return 0xFFFF if over
  if (Max < 0x7FFF || Min < 0x0001) {
    Sample = Max - Min;

    // return value as mV RMS
    return ((Sample * Volts_Conv) * 0.3535);

    // return RAW A/D values
    //return Sample;
  }
  else {
    Sample = 0xFFFF;
  }


} // end of function


