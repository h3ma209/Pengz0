// src/menus/fake_ap_menu.cpp
#include "fake_ap_menu.h"
#include "../../menus/main_menu.h" // Back to main menu
#include "../../hardware.h"
#include "../../wifi/fake_ap/fake_ap.h" // Include to use startFakeAP and stopFakeAP functions
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern bool inFakeAPMenu;
extern uint8_t fakeAPIndex;
extern bool fakeAPEnabled;
extern bool inWiFiMenu; // Actually Main Menu now but used to control menu level
extern uint8_t wifiIndex; //Actually selectedIndex in Main Menu now but used to control menu level index
extern uint8_t selectedIndex;


void showFakeAPMenu(Adafruit_SSD1306 &display) {
    Serial.println("showFakeAPMenu() function called from fake_ap_menu.cpp"); // ADDED
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

    const char *fakeAPOptions[3] = {"Turn On", "Turn Off", "Back"}; // Fake AP Submenu options

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

void handleFakeAPMenuNavigation(Adafruit_SSD1306 &display) {
    static uint8_t lastStateSelectFakeAP = HIGH;
    static uint8_t currentStateSelectFakeAP = digitalRead(BUTTON_SELECT);
    static uint8_t lastStateNextFakeAP = HIGH;      // Track state of NEXT button
    static uint8_t currentStateNextFakeAP = digitalRead(BUTTON_NEXT); // Read NEXT button state

    // --- Handle SELECT button ---
    if (currentStateSelectFakeAP == LOW && lastStateSelectFakeAP == HIGH) {
        Serial.println("FakeAP Menu Select button pressed (LOW) from fake_ap_menu.cpp");
        Serial.print("FakeAP Selected Index: ");
        Serial.println(fakeAPIndex);
        switch (fakeAPIndex) {
            case 0: // "Turn On" selected
                Serial.println("Turn On Fake AP selected from fake_ap_menu.cpp");
                startFakeAP();
                showFakeAPMenu(display);
                break;
            case 1: // "Turn Off" selected
                Serial.println("Turn Off Fake AP selected from fake_ap_menu.cpp");
                stopFakeAP();
                showFakeAPMenu(display);
                break;
            case 2: // "Back" option
                Serial.println("Back option selected in FakeAP Menu from fake_ap_menu.cpp");
                inFakeAPMenu = false;
                inWiFiMenu = false; //Actually Main Menu now
                showMainMenu(selectedIndex, display);
                fakeAPIndex = 0;
                break;
            default:
                Serial.println("Unexpected fakeAPIndex in handleFakeAPMenuNavigation from fake_ap_menu.cpp");
                break;
        }
        delay(200); // Debounce delay (consider making this non-blocking if needed)
    }
    lastStateSelectFakeAP = currentStateSelectFakeAP; // Update SELECT button last state


    // --- Handle NEXT button ---  <--- ADD THIS SECTION
    if (currentStateNextFakeAP == LOW && lastStateNextFakeAP == HIGH) {
        Serial.println("FakeAP Menu NEXT button pressed (LOW)");
        if (fakeAPIndex < 2) { // Assuming 3 options (0, 1, 2 index)
            fakeAPIndex++;
        } else {
            fakeAPIndex = 0; // Wrap around to the first option
        }
        Serial.print("fakeAPIndex after NEXT: ");
        Serial.println(fakeAPIndex);
        showFakeAPMenu(display); // Update display to show new selection
    }
    lastStateNextFakeAP = currentStateNextFakeAP;      // Update NEXT button last state
}