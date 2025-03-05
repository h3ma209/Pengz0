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
#include "wifi/evil_twin/clone_ap/clone_ap_menu.h"

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

  showMainMenu(selectedIndex, display);  // Show menu at startup
}

// Loop Function
void loop() {
  handleMainMenuNavigation(selectedIndex, inSettings, inWiFiMenu, display);
  handleClientRequest(); // Handle captive portal and client requests from wifi.cpp
}