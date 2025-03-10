// src/wifi/wifi.cpp
#include "wifi.h"
#include "evil_twin/clone_ap/clone_ap.h"
#include "fake_ap/fake_ap.h" // Include fake_ap.h to use startFakeAP and stopFakeAP
#include "../hardware.h"
#include <Arduino.h>


extern Adafruit_SSD1306 display;
extern bool inWiFiMenu;
extern uint8_t wifiIndex;
extern uint8_t selectedIndex;
extern uint8_t settingIndex;
extern bool inSettings;
extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern char apSSID[50];;
extern const char *apPassword;
extern String captivePortalPage;
extern bool inFakeAPMenu;
extern uint8_t fakeAPIndex;
extern bool fakeAPEnabled;

// REMOVE THESE DEFINITIONS FROM wifi.cpp - They are DEFINED in main.cpp
// bool inCloneAPMenu = false;
// uint8_t cloneAPIndex = 0;
// std::vector<WiFiNetwork> scannedNetworks;
// bool inEvilTwinMenu = false;
// uint8_t evilTwinIndex = 0;


// Placeholder for handling client requests if needed - currently handled in main loop
void handleClientRequest() {
    if (fakeAPEnabled && WiFi.softAPgetStationNum() > 0) { // Only process requests if FakeAP is enabled
        dnsServer.processNextRequest();
        webServer.handleClient();
    }
}