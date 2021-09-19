/***************************************************************************************************************************/
/*
    A simple library that reads an MLP201077 - 4-channel CT sesnor boards
    Based On:   EmonLib https://github.com/openenergymonitor/EmonLib
    Auther:     David Mottram
    Updated:    6th September 20201
*/
/***************************************************************************************************************************/

#ifndef MLP201077_Lib
#define MLP201077_Lib

// check of legacy IDE
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class MLP201077
{
  public:

    MLP201077(int address, double _ICAL);
    byte test_AD(void);                                           // test if ADS1115 is present at selected I2C address
    void set_mux(int mux_channel);                                // select the mux input
    uint16_t read_AD(void);                                       // read a single sample from the A/D data buffer
    double power_sample(int mux_channel);

  private:

    // I2C address for ADS chip
    int _ADS_Address;

    // ADS1115 registers
    #define Data_Reg 0x00                                         // data register 16bit
    #define Config_Reg 0x01                                       // config register 16bit

    #define Samples_Taken 400                                     // sample to be taken per channel, 1000 is approx 100mS or 5 cycles @ 50Hz
    int8_t ADS_Input = 0;                                         // A/D channel select
    const float Calibration = 1;                                  // calibration value
    const float Volts_Conv = ((2.048 / 0.512) / Calibration);     // volts conversion value A/D 12-bit

    // new bits
    double ICAL;
    int sampleI;                                                  // sample_ holds the raw analog read value

    double offsetI[4] = {16400, 16400, 16400, 16400} ;            // low-pass filter output, one per channel. 16400 is a good place to start
    double Irms;                                                  // useful value variables
    double filteredI;                                             // filtered_ is the raw analog value minus the DC offset
    double sqI, sumI;                                             // sq = squared, sum = Sum
    #define SupplyVoltage 3.3                                     // supply voltage 3.3V
    #define ADC_COUNTS 65536                                      // 16-bit A/D

    // config settings
    #define AIN0 0x4000                                           // mux selection
    #define AIN1 0x5000                                           // mux selection
    #define AIN2 0x6000                                           // mux selection
    #define AIN3 0x7000                                           // mux selection
    #define Voltage_Range 0x0400                                  // range: +/-2.048V
    #define Op_Mode_Cont 0x0000                                   // operating mode: continuous conversion
    #define Data_Rate 0x00E0                                      // speed: 860 SPS (1.16mS per conversion)
    #define Comp_Mode 0x0000                                      // comparator mode: ready after one conversion

};
#endif
