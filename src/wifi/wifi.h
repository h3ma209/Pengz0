// src/wifi/wifi.h
#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Adafruit_SSD1306.h> // Include for display usage
#include <Wire.h> // Include for Wire usage if needed in wifi functions
#include "fake_ap/fake_ap.h"
#include "evil_twin/clone_ap/clone_ap.h"

// External variables (defined in main sketch) that wifi code might need to access
extern Adafruit_SSD1306 display;
extern bool inWiFiMenu;
extern uint8_t wifiIndex;
extern uint8_t selectedIndex;
extern uint8_t settingIndex;
extern bool inSettings;
extern bool inFakeAPMenu;      // ADD: Flag to track if we are in the Fake AP Menu
extern uint8_t fakeAPIndex;     // ADD: Tracks the currently selected Fake AP menu item

extern bool inCloneAPMenu;      // ADD: Flag to track if we are in the Clone AP Menu
extern uint8_t cloneAPIndex;
extern std::vector<WiFiNetwork> scannedNetworks;

extern bool inEvilTwinMenu;
extern uint8_t evilTwinIndex;


// WiFi and AP related variables
extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern const char *apSSID;
extern const char *apPassword;
extern const char *captivePortalPage;
extern bool fakeAPEnabled;     // ADD: Flag to track if Fake AP is enabled


void handleClientRequest(); // If needed to be called from main loop


#endif // WIFI_H