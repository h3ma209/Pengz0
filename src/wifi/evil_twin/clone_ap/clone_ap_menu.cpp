// src/wifi/evil_twin/clone_ap/clone_ap_menu.cpp
#include "clone_ap_menu.h"
#include "../../../hardware.h"
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "../../../wifi/wifi.h" // For WiFiNetwork struct and cloneWiFiNetwork function
#include "../evil_twin_menu.h"

extern bool inCloneAPMenu;
extern uint8_t cloneAPIndex;
extern std::vector<WiFiNetwork> scannedNetworks;
extern bool inEvilTwinMenu;
extern uint8_t evilTwinIndex;
extern bool inWiFiMenu; // Actually Main Menu now but used to control menu level
extern uint8_t wifiIndex; //Actually selectedIndex in Main Menu now but used to control menu level index
extern uint8_t selectedIndex;
// Function to display the Clone AP Network Menu
void showCloneAPNetworkMenu(Adafruit_SSD1306 &display) {
    Serial.println("\n--- showCloneAPNetworkMenu() CALLED ---"); // Added entry print
    Serial.print("Current cloneAPIndex: ");
    Serial.println(cloneAPIndex); // Print index at start
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(10, 10);
    display.println("Clone AP - Select Network");

    if (scannedNetworks.empty()) {
        display.setCursor(10, 20);
        display.println("No networks found!");
        display.display();
        Serial.println("No networks found, showCloneAPNetworkMenu() END"); // Exit print for no networks case
        return;
    }

    for (size_t i = 0; i < scannedNetworks.size(); ++i) {
        if (i == cloneAPIndex) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        } else {
            display.setTextColor(SSD1306_WHITE);
        }
        display.setCursor(10, 20 + (i * 10));
        display.println(scannedNetworks[i].ssid);
    }

    if (cloneAPIndex == scannedNetworks.size()) {
        display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
        display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(10, 20 + (scannedNetworks.size() * 10));
    display.println("Back");

    display.display();
    Serial.println("--- showCloneAPNetworkMenu() END ---"); // Added exit print
}

void handleCloneAPNetworkMenuNavigation(Adafruit_SSD1306 &display) {
    Serial.println("\n--- handleCloneAPNetworkMenuNavigation() CALLED ---"); // Added entry print
    Serial.print("Current cloneAPIndex (start of function): ");
    Serial.println(cloneAPIndex); // Print index at start

    static uint8_t lastStateNextCloneAP = HIGH;
    uint8_t currentStateNextCloneAP = digitalRead(BUTTON_NEXT);
    static uint8_t lastStateSelectCloneAP = HIGH;
    uint8_t currentStateSelectCloneAP = digitalRead(BUTTON_SELECT);

    Serial.print("BUTTON_NEXT state: ");
    Serial.println(currentStateNextCloneAP); // Button states
    Serial.print("BUTTON_SELECT state: ");
    Serial.println(currentStateSelectCloneAP);

    if (currentStateNextCloneAP == LOW && lastStateNextCloneAP == HIGH) {
        Serial.println("CloneAP Menu NEXT button pressed (LOW)");
        if (cloneAPIndex < scannedNetworks.size()) {
            cloneAPIndex++;
        } else {
            cloneAPIndex = 0;
        }
        Serial.print("cloneAPIndex after NEXT: ");
        Serial.println(cloneAPIndex); // Print index after NEXT
        showCloneAPNetworkMenu(display);
    }

    if (currentStateSelectCloneAP == LOW && lastStateSelectCloneAP == HIGH) {
        Serial.println("CloneAP Menu SELECT button pressed (LOW)");
        Serial.print("cloneAPIndex before SELECT action: ");
        Serial.println(cloneAPIndex); // Print index before SELECT

        if (cloneAPIndex < scannedNetworks.size()) {
            WiFiNetwork selectedNetwork = scannedNetworks[cloneAPIndex];
            Serial.print("Case: Network selected - SSID: ");
            Serial.println(selectedNetwork.ssid);
            Serial.println("Setting inCloneAPMenu = false, inEvilTwinMenu = true");
            cloneWiFiNetwork(selectedNetwork);

            inCloneAPMenu = false;
            inEvilTwinMenu = true;
            showEvilTwinMenu(display);
            cloneAPIndex = 0;

        } else if (cloneAPIndex == scannedNetworks.size()) {
            Serial.println("Case: Back option selected");
            Serial.println("Setting inCloneAPMenu = false, inEvilTwinMenu = true");
            inCloneAPMenu = false;
            inEvilTwinMenu = true;
            showEvilTwinMenu(display);
            cloneAPIndex = 0;
        } else {
            Serial.println("Default case: Unexpected cloneAPIndex!");
        }
    }

    lastStateNextCloneAP = currentStateNextCloneAP;
    lastStateSelectCloneAP = currentStateSelectCloneAP;
    Serial.println("--- handleCloneAPNetworkMenuNavigation() END ---\n"); // Added exit print
}