// src/wifi/evil_twin/evil_twin_menu.cpp
#include "evil_twin_menu.h"
#include "clone_ap/clone_ap_menu.h"
#include "../../menus/main_menu.h" // Back to main menu
#include "../../hardware.h"
#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern bool inEvilTwinMenu;
extern uint8_t evilTwinIndex;
extern bool inCloneAPMenu;
extern bool inWiFiMenu; // Actually Main Menu now but used to control menu level
extern uint8_t wifiIndex; //Actually selectedIndex in Main Menu now but used to control menu level index
extern uint8_t selectedIndex;

void showEvilTwinMenu(Adafruit_SSD1306 &display) {
    Serial.println("\n--- showEvilTwinMenu() CALLED ---"); // Added entry print
    Serial.print("Current evilTwinIndex: ");
    Serial.println(evilTwinIndex); // Print index at start
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(10, 10);
    display.println("Evil Twin Options");

    const char *evilTwinOptions[3] = {"Clone AP", "Captive Portal", "Back"};

    for (uint8_t i = 0; i < 3; i++) {
        if (i == evilTwinIndex) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        } else {
            display.setTextColor(SSD1306_WHITE);
        }
        display.setCursor(10, 20 + (i * 10));
        display.println(evilTwinOptions[i]);
    }
    display.display();
    Serial.println("--- showEvilTwinMenu() END ---"); // Added exit print
}

void handleEvilTwinMenuNavigation(Adafruit_SSD1306 &display) {
    Serial.println("\n--- handleEvilTwinMenuNavigation() CALLED ---"); // Added entry print
    Serial.print("Current evilTwinIndex (start of function): ");
    Serial.println(evilTwinIndex); // Print index at start

    static uint8_t lastStateNextEvilTwin = HIGH;
    static uint8_t currentStateNextEvilTwin = digitalRead(BUTTON_NEXT);
    static uint8_t lastStateSelectEvilTwin = HIGH;
    static uint8_t currentStateSelectEvilTwin = digitalRead(BUTTON_SELECT);

    Serial.print("BUTTON_NEXT state: ");
    Serial.println(currentStateNextEvilTwin); // Button states
    Serial.print("BUTTON_SELECT state: ");
    Serial.println(currentStateSelectEvilTwin);

    if (currentStateNextEvilTwin == LOW && lastStateNextEvilTwin == HIGH) {
        Serial.println("EvilTwin Menu NEXT button pressed (LOW)");
        if (evilTwinIndex < 2) {
            evilTwinIndex++;
        } else {
            evilTwinIndex = 0;
        }
        Serial.print("evilTwinIndex after NEXT: ");
        Serial.println(evilTwinIndex); // Print index after NEXT
        showEvilTwinMenu(display);
    }

    if (currentStateSelectEvilTwin == LOW && lastStateSelectEvilTwin == HIGH) {
        Serial.println("EvilTwin Menu SELECT button pressed (LOW)");
        Serial.print("evilTwinIndex before SELECT action: ");
        Serial.println(evilTwinIndex); // Print index before SELECT action

        switch (evilTwinIndex) {
            case 0: // "Clone AP" selected
                Serial.println("Case 0: Clone AP selected");
                Serial.println("Setting inEvilTwinMenu = false, inCloneAPMenu = true");
                inEvilTwinMenu = false;
                inCloneAPMenu = true;
                showCloneAPNetworkMenu(display);
                break;
            case 1: // "Captive Portal" selected
                Serial.println("Case 1: Captive Portal selected");
                Serial.println("Going back to EvilTwinMenu (Captive Portal not implemented)");
                showEvilTwinMenu(display); // For now, go back to Evil Twin menu
                break;
            case 2: // "Back" option
                Serial.println("Case 2: Back option selected");
                Serial.println("Setting inEvilTwinMenu = false, inWiFiMenu = false"); //Actually Main Menu now
                inEvilTwinMenu = false;
                inWiFiMenu = false; // Corrected this line - Back to Main Menu not WiFi Menu
                showMainMenu(selectedIndex, display); // Back to main menu
                evilTwinIndex = 0;
                break;
            default:
                Serial.println("Default case: Unexpected evilTwinIndex!");
                break;
        }
    }
    lastStateNextEvilTwin = currentStateNextEvilTwin;
    lastStateSelectEvilTwin = currentStateSelectEvilTwin;
    Serial.println("--- handleEvilTwinMenuNavigation() END ---\n"); // Added exit print
}