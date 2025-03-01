#include "fake_ap.h"
#include <Arduino.h> // For Serial and ESP8266 functions
#include <ESP8266WiFi.h> // For WiFi functions
#include <DNSServer.h>   // For DNSServer
#include <ESP8266WebServer.h> // For WebServer
#include <Adafruit_SSD1306.h> // Include for display usage
#include <Adafruit_GFX.h> // Include for display usage
#include "../bitmaps/kawaski.h" // Include for bitmap usage


// External variables - Declarations (these are DEFINED in main.cpp)
extern DNSServer dnsServer;
extern ESP8266WebServer webServer;
extern const char* apSSID;
extern const char* apPassword;
extern const char* captivePortalPage;
extern bool fakeAPEnabled;
extern Adafruit_SSD1306 display;



void displayKawaskiBitmap(){
  Serial.println("Displaying Kawasaki Bitmap from fake_ap.cpp");
  display.clearDisplay();
  display.drawBitmap(0,0,kawaskiBitmap, 128, 64, SSD1306_WHITE);
  display.display();
  Serial.println("Kawasaki Bitmap displayed from fake_ap.cpp");
  delay(1000);
  display.clearDisplay();
  display.display();
  Serial.println("Display cleared from fake_ap.cpp");
}

// Start Fake Access Point
void startFakeAP() {
  if (fakeAPEnabled) {
    Serial.println("Fake AP already enabled, ignoring start request from fake_ap.cpp.");
    return; // Do not restart if already enabled
  }
  displayKawaskiBitmap();
  Serial.println("Starting Fake AP from fake_ap.cpp...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);  // Start AP with no password
  Serial.print("Fake AP Started. IP Address: ");
  Serial.println(WiFi.softAPIP());
  dnsServer.start(53, "*", WiFi.softAPIP());
  webServer.on("/", HTTP_GET, []() {
    webServer.send(200, "text/html", captivePortalPage);
  });
  webServer.begin();
  fakeAPEnabled = true; // Set flag to enabled
  Serial.println("FakeAPEnabled set to true in fake_ap.cpp");
}

// Stop Fake Access Point
void stopFakeAP() {
  if (!fakeAPEnabled) {
    Serial.println("Fake AP already disabled, ignoring stop request from fake_ap.cpp.");
    return; // Do not stop if already disabled
  }
  Serial.println("Stopping Fake AP from fake_ap.cpp...");
  webServer.stop();
  dnsServer.stop();
  WiFi.softAPdisconnect(true); // Disconnect soft AP, and reset DHCP server
  WiFi.mode(WIFI_STA); // Return to station mode (or WIFI_OFF, depending on desired default)
  fakeAPEnabled = false; // Set flag to disabled
  Serial.println("Fake AP stopped in fake_ap.cpp.");
  Serial.println("FakeAPEnabled set to false in fake_ap.cpp");
}