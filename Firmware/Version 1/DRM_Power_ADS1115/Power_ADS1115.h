/***************************************************************************************************************************/
/*
    A simple library that reads an ADS1115 A/D converter connected via I2C.
    Auther:   David Mottram
    Updated:  2nd April 2020
*/
/***************************************************************************************************************************/

#ifndef ADS1115_Lib
#define ADS1115_Lib

// check of legacy IDE
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// https://www.arduino.cc/en/Reference/Wire
#include <Wire.h>


class ADS1115
{
  public:
    
    // Add a constructor
    ADS1115(int address);

    // functions
    byte test_AD(void);                                           // test if ADS1115 is present at selected I2C address
    void set_mux(int mux_channel);                                // select the mux input
    uint16_t read_AD(void);                                       // read a single sample from the A/D data buffer
    uint16_t power_sample(int mux_channel, int samples);          // select the mux input and take several samples

  private:

    // I2C address for ADS chip
    int _ADS_Address;

    // ADS1115 registers
    #define Data_Reg 0x00                                         // data register 16bit
    #define Config_Reg 0x01                                       // config register 16bit

    // config settings
    #define AIN0 0x4000                                           // mux selection
    #define AIN1 0x5000                                           // mux selection
    #define AIN2 0x6000                                           // mux selection
    #define AIN3 0x7000                                           // mux selection
    #define Voltage_Range 0x0400                                  // range: +/-2.048V
    #define Op_Mode_Cont 0x0000                                   // operating mode: continuous conversion
    #define Data_Rate 0x00E0                                      // speed: 860 SPS (1.16mS per conversion)
    #define Comp_Mode 0x0000                                      // comparator mode: ready after one conversion
    const float Calibration = 1;
    const float Volts_Conv = ((2.048 / 32.768) / Calibration);    // volts conversion value


};
#endif
