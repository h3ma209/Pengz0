#include "wifi_menu.h"
#include "../wifi/fake_ap/fake_ap_menu.h" // Include for Fake AP Menu
#include "../wifi/evil_twin/evil_twin_menu.h" // Include for Evil Twin Menu
#include "main_menu.h"    // For going back to Main Menu
#include "../hardware.h"
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern bool inWiFiMenu;
extern uint8_t wifiIndex;
extern bool inMainMenu; // We'll need to control Main Menu visibility
extern uint8_t selectedIndex; // Main Menu selected index

extern bool inFakeAPMenu; // To manage Fake AP Menu state
extern uint8_t fakeAPIndex; // Fake AP Menu index

extern bool inEvilTwinMenu; // To manage Evil Twin Menu state
extern uint8_t evilTwinIndex; // Evil Twin Menu index


void showWiFiMenu(Adafruit_SSD1306 &display) {
    Serial.println("showWiFiMenu() called from wifi_menu.cpp");
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(10, 10);
    display.println("WiFi Menu");

    const char *wifiOptions[4] = {"Fake AP", "Evil Twin", "Scan Networks (TBD)", "Back"};

    for (uint8_t i = 0; i < 4; i++) {
        if (i == wifiIndex) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        } else {
            display.setTextColor(SSD1306_WHITE);
        }
        display.setCursor(10, 20 + (i * 10));
        display.println(wifiOptions[i]);
    }
    display.display();
}

void handleWiFiMenuNavigation(Adafruit_SSD1306 &display) {
    static uint8_t lastStateNextWiFi = HIGH;
    static uint8_t lastStateSelectWiFi = HIGH;
    uint8_t currentStateNextWiFi = digitalRead(BUTTON_NEXT);
    uint8_t currentStateSelectWiFi = digitalRead(BUTTON_SELECT);

    // Serial.print("BUTTON_NEXT state: "); // <---- ADD THIS PRINT - button state
    // Serial.println(currentStateNextWiFi);
    // Serial.print("BUTTON_SELECT state: "); // <---- ADD THIS PRINT - button state
    // Serial.println(currentStateSelectWiFi);

    if (currentStateNextWiFi == LOW && lastStateNextWiFi == HIGH) {
        Serial.println("WiFi Menu NEXT button pressed (LOW)");
        if (wifiIndex < 3) {
            wifiIndex++;
        } else {
            wifiIndex = 0;
        }
        showWiFiMenu(display);
    }

    if (currentStateSelectWiFi == LOW && lastStateSelectWiFi == HIGH) {
        Serial.println("WiFi Menu SELECT button pressed (LOW)");
        switch (wifiIndex) {
            case 0: // "Fake AP"
                Serial.println("Fake AP selected in WiFi Menu");
                inWiFiMenu = false; // Exit WiFi Menu
                inFakeAPMenu = true;  // Enter Fake AP Menu
                showFakeAPMenu(display);
                break;
            case 1: // "Evil Twin"
                Serial.println("Evil Twin selected in WiFi Menu");
                inWiFiMenu = false;
                inEvilTwinMenu = true;
                showEvilTwinMenu(display);
                break;
            case 2: // "Scan Networks (TBD)" - Placeholder for future functionality
                Serial.println("Scan Networks selected (TBD) - WiFi Menu");
                showWiFiMenu(display); // For now, stay in WiFi Menu
                break;
            case 3: // "Back"
                Serial.println("Back selected in WiFi Menu");
                inWiFiMenu = false;    // Exit WiFi Menu
                inMainMenu = true;     // Go back to Main Menu
                showMainMenu(selectedIndex, display);
                wifiIndex = 0;         // Reset WiFi menu index
                break;
            default:
                Serial.println("Unexpected wifiIndex in WiFi Menu");
                break;
        }
    }

    lastStateNextWiFi = currentStateNextWiFi;
    lastStateSelectWiFi = currentStateSelectWiFi;
}