// src/wifi/fake_ap/fake_ap.h
#ifndef FAKE_AP_H
#define FAKE_AP_H

#include <ESP8266WiFi.h> // Already included in wifi.h, but good practice to include here if directly used
#include <DNSServer.h>   // Already included in wifi.h, but good practice to include here if directly used
#include <ESP8266WebServer.h> // Already included in wifi.h, but good practice to include here if directly used
#include <Adafruit_SSD1306.h> // Forward declaration needed

// External variables (defined in main sketch) that Fake AP code needs to access
extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern char apSSID[50];;
extern const char *apPassword;
extern String captivePortalPage;
extern bool fakeAPEnabled;
extern Adafruit_SSD1306 display; // For potential display updates from FakeAP code if needed


// Function Prototypes for Fake AP functionality
void startFakeAP();
void stopFakeAP();
void displayKawaskiBitmap();
void displayBeegYoshiBitmap();
void loopAP();


#endif // FAKE_AP_H