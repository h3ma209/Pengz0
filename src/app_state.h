#ifndef APP_STATE_H
#define APP_STATE_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <vector>
#include "wifi/evil_twin/clone_ap/clone_ap.h"

// OLED
extern Adafruit_SSD1306 display;

// Menu navigation flags / indices
extern bool inMainMenu;
extern bool inSettings;
extern bool inWiFiMenu;
extern bool inFakeAPMenu;
extern bool inEvilTwinMenu;
extern bool inCloneAPMenu;
extern bool inCaptivePortalMenu;

extern uint8_t selectedIndex;
extern uint8_t settingIndex;
extern uint8_t wifiIndex;
extern uint8_t fakeAPIndex;
extern uint8_t evilTwinIndex;
extern uint8_t cloneAPIndex;
extern uint8_t captivePortalIndex;

// Fake AP / captive portal
extern bool fakeAPEnabled;
extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern char apSSID[50];
extern const char *apPassword;
extern String captivePortalPage;
extern int portalIndex;

extern std::vector<WiFiNetwork> scannedNetworks;

#endif
