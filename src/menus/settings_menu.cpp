// src/menus/settings_menu.cpp
#include "settings_menu.h"
#include "main_menu.h" // For transitioning back to Main Menu
#include "../hardware.h"
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern bool inSettings;
extern uint8_t settingIndex;
extern uint8_t selectedIndex; // For returning to main menu

// Function to display the Settings screen
void showSettings(Adafruit_SSD1306 &display) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(10, 10);
    display.println("Settings");

    const char *settingOptions[4] = {"Option 1", "Option 2", "Option 3", "Back"};

    // Loop through the settings options and display them
    for (uint8_t i = 0; i < 4; i++) {
        if (i == settingIndex) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Highlighted item
        } else {
            display.setTextColor(SSD1306_WHITE);
        }

        display.setCursor(10, 20 + (i * 10));
        display.println(settingOptions[i]);
    }

    display.display();
}

// Function to handle navigation within the Settings menu
void handleSettingsNavigation(Adafruit_SSD1306 &display) {
    static uint8_t lastStateSelectSettings = HIGH;
    static uint8_t currentStateSelectSettings = digitalRead(BUTTON_SELECT);
    static uint8_t lastStateNextSettings = HIGH;
    static uint8_t currentStateNextSettings = digitalRead(BUTTON_NEXT);

    if (currentStateNextSettings == LOW && lastStateNextSettings == HIGH) {
        Serial.println("Settings Menu: Next button pressed (LOW)");
        settingIndex = (settingIndex + 1) % 4;
        showSettings(display);
        delay(200); // Debounce delay
    }


    if (currentStateSelectSettings == LOW && lastStateSelectSettings == HIGH) {
        Serial.println("Settings Menu: Select button pressed (LOW)");
        if (settingIndex == 3) { // "Back" option
            inSettings = false;
            showMainMenu(selectedIndex, display); // Go back to main menu
            settingIndex = 0; // Reset settings index
            Serial.println("Going back to Main Menu from Settings");
        } else {
            Serial.print("Settings Option selected: ");
            Serial.println(settingIndex + 1); // Option 1, 2, or 3
            // Add functionality for settings options here if needed
        }
        delay(200); // Debounce delay
    }
    lastStateSelectSettings = currentStateSelectSettings;
    lastStateNextSettings = currentStateNextSettings;
}