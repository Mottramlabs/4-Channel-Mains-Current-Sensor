/*
    Wifi settings and IP address
*/

// select Static IP Mode, uncomment below to enable
#ifdef Fixed_IP
#define STATIC_IP
#endif
IPAddress ip(192, 168, 1, 199);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Wifi access point and passwords
const char* SSID_1 = "name";
const char* Password_1 = "password";

const char* SSID_2 = "name";
const char* Password_2 = "password";

const char* SSID_3 = "name";
const char* Password_3 = "password";

const char* SSID_4 = "name";
const char* Password_4 = "password";
