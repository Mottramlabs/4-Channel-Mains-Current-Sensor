/***************************************************************************************************************************/
/*
    A simple library that reads an MLP201077 - 4-channel CT sesnor boards
    Based On:   EmonLib https://github.com/openenergymonitor/EmonLib
    Auther:     David Mottram
    Updated:    6th September 20201
*/
/***************************************************************************************************************************/

#include <ESP8266WiFi.h>              // needed for EPS8266
#include <WiFiClient.h>               // WiFi client

// I/O items
#define Network_LED 14
#define Run_LED 16
#define Speaker 13
#define Test 2

// sound library
#include <ESP8266_Tones.h>
ESP8266_Tones Mytone(Speaker);

// A/D
#define ADS1115_I2C 0x48                            // ADS1115 I2C address
#define Cal_value 1500

// library for the MLP201077 PCB
#include <MLP201077.h>
// make an instance of MLP201077
MLP201077 My_PCB(ADS1115_I2C, Cal_value);

int8_t ADS_Input = 0;                               // A/D channel select
double Value[4] = {0, 0, 0, 0};                     // array for results

void setup() {

  // start the serial connection
  Serial.begin(115200); Serial.println(""); Serial.println(""); Serial.println("Up and Clackin!");
  Serial.println(__FILE__);

  // setup I/O
  pinMode(Run_LED, OUTPUT); digitalWrite(Run_LED, HIGH);
  pinMode(Network_LED, OUTPUT); digitalWrite(Network_LED, HIGH);
  pinMode(Test, OUTPUT); digitalWrite(Test, HIGH);

  // test the A/D converter
  if (My_PCB.test_AD() == 0) {
    Serial.println("ADS1115 Present and OK");
    // speaker test, play a tone for 200mS
    analogWriteFreq(4000);                          // select frequency
    analogWrite(Speaker, 512);                      // 50/50 duty cycle
    delay(200);                                     // period
    analogWrite(Speaker, 0);                        // off
    pinMode(Speaker, INPUT);
  } // end of if
  else {
    Serial.println("ADS1115 Failed!");
    // speaker test, play a tone for 200mS
    analogWriteFreq(400);                           // select frequency
    analogWrite(Speaker, 512);                      // 50/50 duty cycle
    delay(500);                                     // period
    analogWrite(Speaker, 0);                        // off
  } // end of else

  Mytone.Tone_Up(Speaker);

  delay(1000);

} // end of setup


void loop() {

  // read A/D values and store in array Value[]
  // these values are representations of Amps (RMS) measured, and still require some calibration
  digitalWrite(Run_LED, LOW);
  // sampling each channel takes around 600mS. 400 samples (13 cycles @50Hz) with about 1.5mS per A/D sample.
  Value[ADS_Input] = My_PCB.power_sample(ADS_Input);
  digitalWrite(Run_LED, HIGH);

  // inc ready for next A/D channel
  ADS_Input++;
  if (ADS_Input > 3) {
    ADS_Input = 0;
  } // end if

  digitalWrite(Network_LED, HIGH);

  // report results
  String Report = String(Value[0]) + ", " + String(Value[1]) + ", " + String(Value[2]) + ", " + String(Value[3]) + "      ";
  Serial.println(Report);  Serial.println("--------------------------------------------");

  // only used to make the LED flash visable
  delay(10);

  digitalWrite(Network_LED, LOW);

} // end of loop
