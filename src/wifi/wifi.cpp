#include "wifi/wifi.h"
#include "wifi/fake_ap.h" // Include fake_ap.h to use startFakeAP and stopFakeAP
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
extern const char* apSSID;
extern const char* apPassword;
extern const char* captivePortalPage;
extern bool inFakeAPMenu;
extern uint8_t fakeAPIndex;
extern bool fakeAPEnabled;


// Function Definitions for WiFi Menu and Navigation (rest remain in wifi.cpp)


void handleWiFiMenuNavigation() {
    uint8_t lastStateSelect = HIGH;
    uint8_t currentStateSelect = digitalRead(BUTTON_SELECT);

    if (currentStateSelect == LOW && lastStateSelect == HIGH) {
        Serial.println("WiFi Menu Select button pressed (LOW)");
        Serial.print("WiFi Selected Index: "); Serial.println(wifiIndex);
        switch (wifiIndex) {
            case 0: // "Fake AP Settings" option selected - Now goes to FakeAP Submenu
                Serial.println("Fake AP Settings option selected - Entering Fake AP Submenu");
                inFakeAPMenu = true;        // Enter Fake AP Submenu
                showFakeAPMenu();           // Show Fake AP Submenu
                break;
            case 1:
                Serial.println("Evil Twin selected");
                break;
            case 2: // Back option
                Serial.println("Back option selected in WiFi Menu");
                inWiFiMenu = false;         // Exit WiFi Menu
                showMenu(selectedIndex);      // Go back to main menu
                wifiIndex = 0;              // Reset wifiIndex
                Serial.print("inWiFiMenu set to: "); Serial.println(inWiFiMenu);
                Serial.print("wifiIndex reset to: "); Serial.println(wifiIndex);
                break;
            default:
                Serial.println("Unexpected wifiIndex in handleWiFiMenuNavigation");
                break;
        }
        delay(200); // Debounce delay
    }
    lastStateSelect = currentStateSelect;
}


void showWiFiMenu() {
  Serial.println("showWiFiMenu() function called from wifi.cpp");
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10, 10);
  display.println("WiFi Options");

  // Changed "Fake AP" to "Fake AP Settings" to indicate submenu
  const char* wifiOptions[3] = { "Fake AP Settings", "Evil Twin", "Back" };

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


void showFakeAPMenu() {
  Serial.println("showFakeAPMenu() function called from wifi.cpp"); // ADDED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Display Fake AP Status
  display.setCursor(10, 0); // Position at the top
  display.print("Status: ");
  if (fakeAPEnabled) {
    display.println("ON");
  } else {
    display.println("OFF");
  }

  display.setCursor(10, 10); // Move cursor down for the menu title
  display.println("Fake AP Menu"); // Title for Fake AP Menu

  const char* fakeAPOptions[3] = { "Turn On", "Turn Off", "Back" }; // Fake AP Submenu options

  // Loop through Fake AP submenu options
  for (uint8_t i = 0; i < 3; i++) {
    if (i == fakeAPIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Highlighted item
    } else {
      display.setTextColor(SSD1306_WHITE);
    }

    display.setCursor(10, 20 + (i * 10));
    display.println(fakeAPOptions[i]);
  }

  display.display();
}


void handleFakeAPMenuNavigation() {
    uint8_t lastStateSelectFakeAP = HIGH;
    uint8_t currentStateSelectFakeAP = digitalRead(BUTTON_SELECT);

    if (currentStateSelectFakeAP == LOW && lastStateSelectFakeAP == HIGH) {
        Serial.println("FakeAP Menu Select button pressed (LOW) from wifi.cpp"); // Added print here
        Serial.print("FakeAP Selected Index: "); Serial.println(fakeAPIndex); // Print fakeAPIndex value
        switch (fakeAPIndex) {
            case 0: // "Turn On" selected
                Serial.println("Turn On Fake AP selected from wifi.cpp");
                startFakeAP(); // Call startFakeAP function - now in fake_ap.cpp
                showFakeAPMenu(); // Refresh display to show "Status: ON" immediately
                break;
            case 1: // "Turn Off" selected
                Serial.println("Turn Off Fake AP selected from wifi.cpp");
                stopFakeAP();  // Call stopFakeAP function - now in fake_ap.cpp
                showFakeAPMenu(); // Refresh display to show "Status: OFF" immediately
                break;
            case 2: // "Back" option
                Serial.println("Back option selected in FakeAP Menu from wifi.cpp");
                inFakeAPMenu = false;      // Exit Fake AP Menu
                showWiFiMenu();            // Go back to WiFi menu
                fakeAPIndex = 0;           // Reset fakeAPIndex when going back
                Serial.print("inFakeAPMenu set to: "); Serial.println(inFakeAPMenu); // Print inFakeAPMenu value after setting
                Serial.print("fakeAPIndex reset to: "); Serial.println(fakeAPIndex); // Print fakeAPIndex value after reset
                break;
            default:
                Serial.println("Unexpected fakeAPIndex in handleFakeAPMenuNavigation from wifi.cpp"); // Added default case print
                break;
        }
        delay(200); // Debounce delay
    }
    lastStateSelectFakeAP = currentStateSelectFakeAP;
}


// Placeholder for handling client requests if needed - currently handled in main loop
void handleClientRequest() {
  if (fakeAPEnabled && WiFi.softAPgetStationNum() > 0) { // Only process requests if FakeAP is enabled
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}