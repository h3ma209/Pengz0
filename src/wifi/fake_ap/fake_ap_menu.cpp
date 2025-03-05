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
    uint8_t lastStateSelectFakeAP = HIGH;
    uint8_t currentStateSelectFakeAP = digitalRead(BUTTON_SELECT);

    if (currentStateSelectFakeAP == LOW && lastStateSelectFakeAP == HIGH) {
        Serial.println("FakeAP Menu Select button pressed (LOW) from fake_ap_menu.cpp"); // Added print here
        Serial.print("FakeAP Selected Index: ");
        Serial.println(fakeAPIndex); // Print fakeAPIndex value
        switch (fakeAPIndex) {
            case 0: // "Turn On" selected
                Serial.println("Turn On Fake AP selected from fake_ap_menu.cpp");
                startFakeAP(); // Call startFakeAP function - now in fake_ap.cpp
                showFakeAPMenu(display); // Refresh display to show "Status: ON" immediately
                break;
            case 1: // "Turn Off" selected
                Serial.println("Turn Off Fake AP selected from fake_ap_menu.cpp");
                stopFakeAP();  // Call stopFakeAP function - now in fake_ap.cpp
                showFakeAPMenu(display); // Refresh display to show "Status: OFF" immediately
                break;
            case 2: // "Back" option
                Serial.println("Back option selected in FakeAP Menu from fake_ap_menu.cpp");
                inFakeAPMenu = false;      // Exit Fake AP Menu
                inWiFiMenu = false; //Actually Main Menu now but used to control menu level to false to go back to main menu
                showMainMenu(selectedIndex, display);            // Go back to main menu
                fakeAPIndex = 0;           // Reset fakeAPIndex when going back
                Serial.print("inFakeAPMenu set to: ");
                Serial.println(inFakeAPMenu); // Print inFakeAPMenu value after setting
                Serial.print("fakeAPIndex reset to: ");
                Serial.println(fakeAPIndex); // Print fakeAPIndex value after reset
                break;
            default:
                Serial.println("Unexpected fakeAPIndex in handleFakeAPMenuNavigation from fake_ap_menu.cpp"); // Added default case print
                break;
        }
        delay(200); // Debounce delay
    }
    lastStateSelectFakeAP = currentStateSelectFakeAP;
}