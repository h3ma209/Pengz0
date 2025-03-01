#ifndef FAKE_AP_H
#define FAKE_AP_H

#include <ESP8266WiFi.h> // Already included in wifi.h, but good practice to include here if directly used
#include <DNSServer.h>   // Already included in wifi.h, but good practice to include here if directly used
#include <ESP8266WebServer.h> // Already included in wifi.h, but good practice to include here if directly used
#include "../bitmaps/kawaski.h" // Include for bitmap usage


class Adafruit_SSD1306;// Forward declaration for Adafruit_SSD1306 only, as headers for DNSServer and ESP8266WebServer are included


// External variables (defined in main sketch) that Fake AP code needs to access
extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern const char* apSSID;
extern const char* apPassword;
extern const char* captivePortalPage;
extern bool fakeAPEnabled;
extern Adafruit_SSD1306 display; // For potential display updates from FakeAP code if needed


// Function Prototypes for Fake AP functionality
void startFakeAP();
void stopFakeAP();
void displayKawaskiBitmap();


#endif // FAKE_AP_H