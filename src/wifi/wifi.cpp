#include "wifi/wifi.h"
#include "../hardware.h" // Include hardware.h for button pin definitions
#include <Arduino.h> // Make sure to include Arduino.h for ESP8266 specific functions like Serial, digitalRead, delay
//#include "../config.h" // If you have a config.h, include it like this - path might adjust

// Variables - moved from main sketch - IMPORTANT: Do NOT define them here, they are defined in main.cpp and declared as extern in wifi.h
// DNSServer dnsServer; // REMOVE definition
// ESP8266WebServer webServer; // REMOVE definition

// Fake AP settings - using defines from main sketch or config.h if moved -  REMOVE definition if you want to keep them in main.cpp
// const char* apSSID = "Fake_AP_123";
// const char* apPassword = "";
// const char* captivePortalPage = "<html><body><h1>Welcome to the Fake AP!</h1><p>You are now connected to the Fake AP.</p></body></html>";

extern Adafruit_SSD1306 display; // Declare display as extern, it's defined in main.cpp
extern bool inWiFiMenu; // Declare inWiFiMenu as extern, defined in main.cpp
extern uint8_t wifiIndex; // Declare wifiIndex as extern, defined in main.cpp
extern uint8_t selectedIndex; // Declare selectedIndex as extern, defined in main.cpp
extern uint8_t settingIndex; // Declare settingIndex as extern, defined in main.cpp
extern bool inSettings; // Declare inSettings as extern, defined in main.cpp
extern DNSServer dnsServer; // Declare dnsServer as extern, defined in main.cpp
extern ESP8266WebServer webServer; // Declare webServer as extern, defined in main.cpp
extern const char* apSSID; // Declare apSSID as extern, defined in main.cpp
extern const char* apPassword; // Declare apPassword as extern, defined in main.cpp
extern const char* captivePortalPage; // Declare captivePortalPage as extern, defined in main.cpp


// Function Definitions - moved and adjusted from main sketch


// Start Fake Access Point
void startFakeAP() {
  Serial.println("Starting Fake AP...");

  // Set the ESP8266 to AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword);  // Start AP with no password

  Serial.print("Fake AP Started. IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Set up the DNS server to redirect all requests to the fake portal
  dnsServer.start(53, "*", WiFi.softAPIP());

  // Set up the web server to serve the captive portal page
  webServer.on("/", HTTP_GET, []() {
    webServer.send(200, "text/html", captivePortalPage);
  });

  webServer.begin();
}

// Placeholder for handling client requests if needed - currently handled in main loop
void handleClientRequest() {
  if (WiFi.softAPgetStationNum() > 0) {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}
void handleWiFiMenuNavigation() { // Example of a function related to wifi menu logic
    uint8_t lastStateSelect = HIGH;
    uint8_t currentStateSelect = digitalRead(BUTTON_SELECT);

    if (currentStateSelect == LOW && lastStateSelect == HIGH) {
        Serial.println("WiFi Menu Select button pressed (LOW)"); // Added print here
        Serial.print("WiFi Selected Index: "); Serial.println(wifiIndex); // Print wifiIndex value
        switch (wifiIndex) {
            case 0:
                Serial.println("Fake AP selected");
                startFakeAP();  // Start the Fake AP
                break;
            case 1:
                Serial.println("Evil Twin selected");
                break;
            case 2: // Back option
                Serial.println("Back option selected in WiFi Menu"); // Added print here
                inWiFiMenu = 0;
                showMenu(selectedIndex);  // Go back to main menu (assuming showMenu is in main sketch)
                wifiIndex = 0; // Reset wifiIndex when going back
                Serial.print("inWiFiMenu set to: "); Serial.println(inWiFiMenu); // Print inWiFiMenu value after setting
                Serial.print("wifiIndex reset to: "); Serial.println(wifiIndex); // Print wifiIndex value after reset
                break;
            default:
                Serial.println("Unexpected wifiIndex in handleWiFiMenuNavigation"); // Added default case print
                break;
        }
        delay(200); // Debounce delay
    }
    lastStateSelect = currentStateSelect;
}


void showWiFiMenu() {
  Serial.println("showWiFiMenu() function called"); // ADDED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10, 10);
  display.println("WiFi Options");

  const char* wifiOptions[3] = { "Fake AP", "Evil Twin", "Back" };

  // Loop through the WiFi options and display them
  for (uint8_t i = 0; i < 3; i++) {
    if (i == wifiIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Highlighted item
    } else {
      display.setTextColor(SSD1306_WHITE);
    }

    display.setCursor(10, 20 + (i * 10));
    display.println(wifiOptions[i]);
  }

  display.display();
}