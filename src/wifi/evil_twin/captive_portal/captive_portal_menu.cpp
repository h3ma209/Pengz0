#include "captive_portal_menu.h"
#include "../../../hardware.h"
#include "../evil_twin_menu.h"

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

extern bool inCaptivePortalMenu;
extern uint8_t captivePortalIndex;
extern bool captivePortalEnabled;

extern bool inEvilTwinMenu;
extern uint8_t evilTwinIndex;


void showCaptivePortalMenu(Adafruit_SSD1306 &display){
    Serial.println("\n --- showCaptivePortalMenu() CALLED ---"); // Added entry print
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Display Captive Portal Status
    display.setCursor(10, 0); // Position at the top
    display.print("Status: ");
    if (captivePortalEnabled) {
        display.println("ON");
    } else {
        display.println("OFF");
    }
    display.setCursor(10, 10); // Move cursor down for the menu title
    display.println("Captive Portal Menu"); // Title for Captive Portal Menu

    const char *captivePortalOptions[5] = {"IQ Login", "Google Login", "MyKomar", "Komar Cap", "Back"}; // Captive Portal Submenu options

    // Loop through Captive Portal submenu options
    for (uint8_t i =0; i < 5; i++){
        if(i == captivePortalIndex){
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Highlighted item
        } else {
            display.setTextColor(SSD1306_WHITE);
        }

        display.setCursor(10, 20 + (i * 10));
        display.println(captivePortalOptions[i]);
    }

    display.display();
}


void handleCaptivePortalMenuNavigation(Adafruit_SSD1306 &display){
    static uint8_t lastStateSelectCaptivePortal = HIGH;
    uint8_t currentStateSelectCaptivePortal = digitalRead(BUTTON_SELECT);
    static uint8_t lastStateNextCaptivePortal = HIGH;
    uint8_t currentStateNextCaptivePortal = digitalRead(BUTTON_NEXT);

    if(currentStateNextCaptivePortal == LOW && lastStateNextCaptivePortal == HIGH){
        if(captivePortalIndex < 4){
            captivePortalIndex++;
        } else {
            captivePortalIndex = 0;
        }
        showCaptivePortalMenu(display);
    }

    if(currentStateSelectCaptivePortal == LOW && lastStateSelectCaptivePortal == HIGH){
        switch(captivePortalIndex){
            case 0:
                Serial.println("IQ Login");
                break;
            case 1:
                Serial.println("Google Login");
                break;
            case 2:
                Serial.println("MyKomar");
                break;
            case 3:
                Serial.println("Komar Cap");
                break;
            case 4:
                inCaptivePortalMenu = false;
                captivePortalIndex = 0;
                inEvilTwinMenu = true;
                showEvilTwinMenu(display);
                break;
        }
    }
    lastStateNextCaptivePortal = currentStateNextCaptivePortal; 
    lastStateSelectCaptivePortal = currentStateSelectCaptivePortal;
}