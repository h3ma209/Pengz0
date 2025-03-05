// src/main.cpp
#include <Arduino.h> // Important for PlatformIO and .cpp files
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "wifi/wifi.h" // Include wifi functionalities
#include "hardware.h"
#include "menus/main_menu.h"
#include "menus/settings_menu.h"
#include "wifi/fake_ap/fake_ap_menu.h"
#include "wifi/evil_twin/evil_twin_menu.h"
#include "wifi/evil_twin/clone_ap/clone_ap_menu.h" // Make sure to include clone_ap_menu.h
#include "menus/wifi_menu.h"     // Include WiFi Menu

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Button Pins - DEFINITIONS (and initializations) in main.cpp
const uint8_t BUTTON_NEXT  = 14;  // Scroll button (e.g., connected to GPIO14) D5
const uint8_t BUTTON_SELECT = 12; // Select button (e.g., connected to GPIO12) D6

uint8_t selectedIndex = 0;
uint8_t settingIndex = 0;
uint8_t wifiIndex = 0;
bool inSettings = false;
bool inWiFiMenu = false;
bool inFakeAPMenu = false;
uint8_t fakeAPIndex = 0;
bool fakeAPEnabled = false;
bool inMainMenu = true; // Control for main menu visibility, start with Main Menu visible
bool inCloneAPMenu = false;
uint8_t cloneAPIndex = 0;
std::vector<WiFiNetwork> scannedNetworks;

bool inEvilTwinMenu = false;
uint8_t evilTwinIndex= 0;

// Captive Portal variables - Definitions (memory allocation)
DNSServer dnsServer;
ESP8266WebServer webServer;

// Fake AP settings - Definitions (memory allocation)
const char* apSSID = "FTBLL EDITZ hema pooppeeee ^vvv^";
const char* apPassword = "";
const char* captivePortalPage = "<html><body><h1>Welcome to the Fake AP!</h1><p>You are now connected to the Fake AP.</p></body></html>";

// OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//--- Extern declarations for variables used in other files ---
extern Adafruit_SSD1306 display; // Defined here, used in menus and wifi
extern bool inWiFiMenu;         // Defined here, used in menus
extern uint8_t wifiIndex;          // Defined here, used in menus
extern uint8_t selectedIndex;    // Defined here, used in menus
extern uint8_t settingIndex;     // Defined here, used in settings menu
extern bool inSettings;          // Defined here, used in settings menu and main menu
extern bool inFakeAPMenu;        // Defined here, used in fake_ap_menu.cpp and wifi_menu.cpp
extern uint8_t fakeAPIndex;         // Defined here, used in fake_ap_menu.cpp and wifi_menu.cpp
extern bool fakeAPEnabled;       // Defined here, used in fake_ap_menu.cpp and wifi.cpp
extern bool inMainMenu;          // Defined here, used in main_menu.cpp and settings_menu.cpp, wifi_menu.cpp
extern bool inCloneAPMenu;       // Defined here, used in clone_ap_menu.cpp, evil_twin_menu.cpp, wifi_menu.cpp
extern uint8_t cloneAPIndex;        // Defined here, used in clone_ap_menu.cpp, wifi_menu.cpp
extern std::vector<WiFiNetwork> scannedNetworks; // Defined here, used in wifi.cpp, clone_ap_menu.cpp
extern bool inEvilTwinMenu;      // Defined here, used in evil_twin_menu.cpp, wifi_menu.cpp, clone_ap_menu.cpp
extern uint8_t evilTwinIndex;       // Defined here, used in evil_twin_menu.cpp, wifi_menu.cpp

void setup() {
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Set up button pins
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);

  showMainMenu(selectedIndex, display);  // Show main menu at startup
}

// Loop Function
void loop() {
  if (inMainMenu) {
    handleMainMenuNavigation(selectedIndex, inSettings, inWiFiMenu, display);
  } else if (inSettings) {
    // handleSettingsMenuNavigation(settingIndex, inSettings, inMainMenu, display); //Go back to Main Menu from settings
  } else if (inWiFiMenu) {
    Serial.println("Entering handleWiFiMenuNavigation");
    handleWiFiMenuNavigation(display); // Handle WiFi Menu navigation
  } else if (inFakeAPMenu) {
    handleFakeAPMenuNavigation(display);
  } else if (inEvilTwinMenu) {
      handleEvilTwinMenuNavigation(display);
  } else if (inCloneAPMenu) {
      handleCloneAPNetworkMenuNavigation(display);
  }
  
}