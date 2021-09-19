#include <ESP8266WiFi.h>              // needed for EPS8266

// I/O items
#define Speaker 13
#define Network_LED 14
#define Run_LED 16

// ADS1115 items
#define ADS1115_I2C 0x48                          // ADS1115 I2C address
#define Samples_Taken 200                         // sample to be taken per channel
int8_t ADS_Input = 0;                             // A/D channel select
int16_t Value[4] = {0, 0, 0, 0};                  // array for results

#include <Power_ADS1115.h>
ADS1115 My_Board(ADS1115_I2C);

// heartbeat timer, time between printed results
#define Heatbeat    1000                          // heartbeat timer value, in mS
long LastMsg = 0;                                 // message sent counter


void setup()
{
  // start the serial connection
  Serial.begin(115200); Serial.println(""); Serial.println(""); Serial.println("Up and Clackin!");

  // setup I/O
  pinMode(Run_LED, OUTPUT); digitalWrite(Run_LED, HIGH);
  pinMode(Network_LED, OUTPUT); digitalWrite(Network_LED, HIGH);
  pinMode(Speaker, OUTPUT); digitalWrite(Speaker, LOW);

  // speaker test, play a tone for 200mS
  analogWriteFreq(4000);                          // select frequency
  analogWrite(Speaker, 512);                      // 50/50 duty cycle
  delay(200);                                     // period
  analogWrite(Speaker, 0);                        // off

  // test the A/D converter
  if (My_Board.test_AD() == 0) {
    Serial.println("ADS1115 Present and OK");
  }
  else {
    Serial.println("ADS1115 Failed!");
  }

  // reset heartbeat timer
  LastMsg = millis();

  digitalWrite(Run_LED, LOW); digitalWrite(Network_LED, LOW);

} // end of setup


void loop()
{

  // read A/D values and store in array Value[]
  digitalWrite(Run_LED, LOW);
  Value[ADS_Input] = My_Board.power_sample(ADS_Input, Samples_Taken);
  digitalWrite(Run_LED, HIGH);

  // inc ready for next A/D channel
  ADS_Input++;
  if (ADS_Input > 3) {
    ADS_Input = 0;
  }

  // only used to make the LED flash visable
  delay(10);

  // headbeat timed out?
  if (millis() - LastMsg > Heatbeat) {

    LastMsg = millis();

    // print the values
    digitalWrite(Network_LED, HIGH);

    String Report = String(Value[0]) + "mV, " + String(Value[1]) + "mV, " + String(Value[2]) + "mV, " + String(Value[3]) + "mV";
    Serial.println(Report);

    // only used to make the LED flash visable
    delay(10);

    digitalWrite(Network_LED, LOW);

  } // end of heartbeat

} // end of loop
