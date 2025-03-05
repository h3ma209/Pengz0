#include "wifi.h"
#include "clone_ap.h"
#include "fake_ap.h" // Include fake_ap.h to use startFakeAP and stopFakeAP
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

bool inCloneAPMenu = false;
uint8_t cloneAPIndex = 0;
std::vector<WiFiNetwork> scannedNetworks;

bool inEvilTwinMenu = false;
uint8_t evilTwinIndex= 0;



// Function Definitions for WiFi Menu and Navigation (rest remain in wifi.cpp)


void handleWiFiMenuNavigation() {
  uint8_t lastStateSelect = HIGH;
  uint8_t currentStateSelect = digitalRead(BUTTON_SELECT);

  if (currentStateSelect == LOW && lastStateSelect == HIGH) {
      Serial.println("WiFi Menu Select button pressed (LOW)");
      Serial.print("WiFi Selected Index: "); Serial.println(wifiIndex);
      switch (wifiIndex) {
          case 0: // "Fake AP Settings"
              Serial.println("Fake AP Settings option selected - Entering Fake AP Submenu");
              inWiFiMenu = false; // IMPORTANT: Exit WiFi Menu state FIRST
              inFakeAPMenu = true;
              showFakeAPMenu();
              Serial.println("Exiting handleWiFiMenuNavigation() - Entering FakeAP Menu"); // Debug print on exit for FakeAP
              return; // IMPORTANT: Add return to exit function after menu transition for FakeAP
          case 1: // "Evil Twin" selected
              Serial.println("Evil Twin selected - Entering Evil Twin Submenu");
              inWiFiMenu = false; // IMPORTANT: Exit WiFi Menu state FIRST
              inEvilTwinMenu = true; // THEN enter Evil Twin Menu state
              showEvilTwinMenu();
              Serial.println("Exiting handleWiFiMenuNavigation() - Entering EvilTwin Menu"); // Debug print on exit for EvilTwin
              return; // IMPORTANT: Add return to exit function after menu transition for EvilTwin
          case 2: // Back option
              Serial.println("Back option selected in WiFi Menu");
              inWiFiMenu = false;
              showMenu(selectedIndex);
              wifiIndex = 0;
              Serial.print("inWiFiMenu set to: "); Serial.println(inWiFiMenu);
              Serial.print("wifiIndex reset to: "); Serial.println(wifiIndex);
              Serial.println("Exiting handleWiFiMenuNavigation() - Back to Main Menu"); // Debug print on exit for Back
              return; // IMPORTANT: Add return to exit function after menu transition for Back
          default:
              Serial.println("Unexpected wifiIndex in handleWiFiMenuNavigation");
              break;
      }
      delay(200); // Debounce delay
  }
  lastStateSelect = currentStateSelect;
  Serial.println("Returned from handleWiFiMenuNavigation() - No action taken"); // Debug print at normal function end when no button pressed
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
void showEvilTwinMenu() {
  Serial.println("\n--- showEvilTwinMenu() CALLED ---"); // Added entry print
  Serial.print("Current evilTwinIndex: "); Serial.println(evilTwinIndex); // Print index at start
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10, 10);
  display.println("Evil Twin Options");

  const char* evilTwinOptions[3] = { "Clone AP", "Captive Portal", "Back" };

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


void handleEvilTwinMenuNavigation() {
  Serial.println("\n--- handleEvilTwinMenuNavigation() CALLED ---"); // Added entry print
  Serial.print("Current evilTwinIndex (start of function): "); Serial.println(evilTwinIndex); // Print index at start

  uint8_t lastStateNextEvilTwin = HIGH;
  uint8_t currentStateNextEvilTwin = digitalRead(BUTTON_NEXT);
  uint8_t lastStateSelectEvilTwin = HIGH;
  uint8_t currentStateSelectEvilTwin = digitalRead(BUTTON_SELECT);

  Serial.print("BUTTON_NEXT state: "); Serial.println(currentStateNextEvilTwin); // Button states
  Serial.print("BUTTON_SELECT state: "); Serial.println(currentStateSelectEvilTwin);

  if (currentStateNextEvilTwin == LOW && lastStateNextEvilTwin == HIGH) {
      Serial.println("EvilTwin Menu NEXT button pressed (LOW)");
      if (evilTwinIndex < 2) {
          evilTwinIndex++;
      } else {
          evilTwinIndex = 0;
      }
      Serial.print("evilTwinIndex after NEXT: "); Serial.println(evilTwinIndex); // Print index after NEXT
      showEvilTwinMenu();
  }

  if (currentStateSelectEvilTwin == LOW && lastStateSelectEvilTwin == HIGH) {
      Serial.println("EvilTwin Menu SELECT button pressed (LOW)");
      Serial.print("evilTwinIndex before SELECT action: "); Serial.println(evilTwinIndex); // Print index before SELECT action

      switch (evilTwinIndex) {
          case 0: // "Clone AP" selected
              Serial.println("Case 0: Clone AP selected");
              Serial.println("Setting inEvilTwinMenu = false, inCloneAPMenu = true");
              inEvilTwinMenu = false;
              inCloneAPMenu = true;
              showCloneAPNetworkMenu();
              break;
          case 1: // "Captive Portal" selected
              Serial.println("Case 1: Captive Portal selected");
              Serial.println("Going back to EvilTwinMenu (Captive Portal not implemented)");
              showEvilTwinMenu(); // For now, go back to Evil Twin menu
              break;
          case 2: // "Back" option
              Serial.println("Case 2: Back option selected");
              Serial.println("Setting inEvilTwinMenu = false, inWiFiMenu = true");
              inEvilTwinMenu = false;
              inWiFiMenu = true;
              showWiFiMenu();
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
void showCloneAPNetworkMenu() {
  Serial.println("\n--- showCloneAPNetworkMenu() CALLED ---"); // Added entry print
  Serial.print("Current cloneAPIndex: "); Serial.println(cloneAPIndex); // Print index at start
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


void handleCloneAPNetworkMenuNavigation() {
  Serial.println("\n--- handleCloneAPNetworkMenuNavigation() CALLED ---"); // Added entry print
  Serial.print("Current cloneAPIndex (start of function): "); Serial.println(cloneAPIndex); // Print index at start

  uint8_t lastStateNextCloneAP = HIGH;
  uint8_t currentStateNextCloneAP = digitalRead(BUTTON_NEXT);
  uint8_t lastStateSelectCloneAP = HIGH;
  uint8_t currentStateSelectCloneAP = digitalRead(BUTTON_SELECT);

  Serial.print("BUTTON_NEXT state: "); Serial.println(currentStateNextCloneAP); // Button states
  Serial.print("BUTTON_SELECT state: "); Serial.println(currentStateSelectCloneAP);

  if (currentStateNextCloneAP == LOW && lastStateNextCloneAP == HIGH) {
      Serial.println("CloneAP Menu NEXT button pressed (LOW)");
      if (cloneAPIndex < scannedNetworks.size()) {
          cloneAPIndex++;
      } else {
          cloneAPIndex = 0;
      }
      Serial.print("cloneAPIndex after NEXT: "); Serial.println(cloneAPIndex); // Print index after NEXT
      showCloneAPNetworkMenu();
  }

  if (currentStateSelectCloneAP == LOW && lastStateSelectCloneAP == HIGH) {
      Serial.println("CloneAP Menu SELECT button pressed (LOW)");
      Serial.print("cloneAPIndex before SELECT action: "); Serial.println(cloneAPIndex); // Print index before SELECT

      if (cloneAPIndex < scannedNetworks.size()) {
          WiFiNetwork selectedNetwork = scannedNetworks[cloneAPIndex];
          Serial.print("Case: Network selected - SSID: "); Serial.println(selectedNetwork.ssid);
          Serial.println("Setting inCloneAPMenu = false, inEvilTwinMenu = true");
          cloneWiFiNetwork(selectedNetwork);

          inCloneAPMenu = false;
          inEvilTwinMenu = true;
          showEvilTwinMenu();
          cloneAPIndex = 0;

      } else if (cloneAPIndex == scannedNetworks.size()) {
          Serial.println("Case: Back option selected");
          Serial.println("Setting inCloneAPMenu = false, inEvilTwinMenu = true");
          inCloneAPMenu = false;
          inEvilTwinMenu = true;
          showEvilTwinMenu();
          cloneAPIndex = 0;
      } else {
          Serial.println("Default case: Unexpected cloneAPIndex!");
      }
  }

  lastStateNextCloneAP = currentStateNextCloneAP;
  lastStateSelectCloneAP = currentStateSelectCloneAP;
  Serial.println("--- handleCloneAPNetworkMenuNavigation() END ---\n"); // Added exit print
}