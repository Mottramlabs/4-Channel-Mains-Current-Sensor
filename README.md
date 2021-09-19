# 4-Channel-Mains-Current-Sensor
A 4-Channel mains sensor with an option for either a Wemos D1 Mini or an ESP-12 WiFi module

###### Firmware Version 1

Using the an external A/D ADS1115 to measure the output of the current transformer, this version measure the peak values of the mains sine wave then calculating the RMS voltage value.

###### Firmware Version 2

This is based of the EmonLib https://github.com/openenergymonitor/EmonLib library. This works different from version 1 and as some digital filtering which improves resolution at lower power levels.

### PCB Layout
![Display-Type-B](https://github.com/Mottramlabs/4-Channel-Mains-Current-Sensor/blob/master/PCB%20and%20CT.jpg?raw=true)

