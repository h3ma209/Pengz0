// src/menus/main_menu.cpp
#include "main_menu.h"
#include "../hardware.h" // Include hardware definitions
#include "../wifi/evil_twin/evil_twin_menu.h"   // For transitioning to Evil Twin Menu
#include "settings_menu.h" // For transitioning to Settings Menu
#include <Arduino.h>      // For digitalRead, Serial, etc.
#include <Adafruit_SSD1306.h> // For display functions
#include "../wifi/fake_ap/fake_ap_menu.h"

// *************************************************************
// NOTE: Global variables related to menu state are still in main.cpp.
// We are passing them as references to these menu functions.
// *************************************************************


// Function to display the main menu
void showMainMenu(uint8_t selectedIndex, Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    const char *menuOptions[5] = {"WiFi", "Radio", "RFID", "Remote", "Setting"};

    for (uint8_t i = 0; i < 5; i++) {
        if (i == selectedIndex) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Highlighted item
        } else {
            display.setTextColor(SSD1306_WHITE);
        }

        display.setCursor(10, 10 + (i * 10));
        display.println(menuOptions[i]);
    }

    display.display();
}

// Function to handle menu navigation with buttons in Main Menu
void handleMainMenuNavigation(uint8_t &selectedIndex, bool &inSettings, bool &inWiFiMenu, Adafruit_SSD1306 &display) {
    static uint8_t lastStateNext = HIGH;
    static uint8_t lastStateSelect = HIGH;

    uint8_t currentStateNext = digitalRead(BUTTON_NEXT);
    uint8_t currentStateSelect = digitalRead(BUTTON_SELECT);

    if (currentStateNext == LOW && lastStateNext == HIGH) {
        Serial.println("Main Menu: Next button pressed (LOW)");
        selectedIndex = (selectedIndex + 1) % 5;
        showMainMenu(selectedIndex, display); // Pass display object
        delay(200); // Debounce delay
    }

    if (currentStateSelect == LOW && lastStateSelect == HIGH) {
        Serial.println("Main Menu: Select button pressed (LOW)");
        switch (selectedIndex) {
            case 0: // WiFi
                inWiFiMenu = true;
                showFakeAPMenu(display); // For now directly go to FakeAP menu for testing //showWiFiMenu(display); // Transition to WiFi Menu - showWiFiMenu needs to be updated to take display
                Serial.println("Entering WiFi Menu from Main Menu");
                break;
            case 1:
                Serial.println("Radio");
                break;
            case 2:
                Serial.println("RFID");
                break;
            case 3:
                Serial.println("Remote");
                break;
            case 4: // Setting
                inSettings = true;
                showSettings(display); // Transition to Settings Menu - showSettings needs to be updated to take display
                Serial.println("Entering Settings Menu from Main Menu");
                break;
        }
        delay(200); // Debounce delay
    }

    lastStateNext = currentStateNext;
    lastStateSelect = currentStateSelect;
}