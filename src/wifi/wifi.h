#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Adafruit_SSD1306.h> // Include for display usage
#include <Wire.h> // Include for Wire usage if needed in wifi functions

// External variables (defined in main sketch) that wifi code might need to access
extern Adafruit_SSD1306 display;
extern bool inWiFiMenu;
extern uint8_t wifiIndex;
extern uint8_t selectedIndex;
extern uint8_t settingIndex; // Add settingIndex as well, if needed in wifi functions
extern bool inSettings; // Add inSettings as well, if needed in wifi functions
extern uint8_t selectedIndex;

// WiFi and AP related variables
extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern const char* apSSID;
extern const char* apPassword;
extern const char* captivePortalPage;

// Function Prototypes
void showWiFiMenu();
void startFakeAP();
void handleClientRequest(); // If needed to be called from main loop
void handleWiFiMenuNavigation(); // Function to handle navigation within wifi menu

#ifdef __cplusplus
extern "C" {
#endif

void showMenu(uint8_t selectedIndex); // Assume showMenu is still in main, declare it here as extern "C" if wifi.cpp needs to call it

#ifdef __cplusplus
}
#endif


#endif // WIFI_H