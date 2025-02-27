#include <Arduino.h> // Important for PlatformIO and .cpp files
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "wifi/wifi.h" // Include wifi functionalities

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BUTTON_NEXT  14  // Scroll button (e.g., connected to GPIO14) D5
#define BUTTON_SELECT 12 // Select button (e.g., connected to GPIO12) D6

uint8_t selectedIndex = 0;
uint8_t settingIndex = 0;
uint8_t wifiIndex = 0;
bool inSettings = false;
bool inWiFiMenu = false;
bool inFakeAPMenu = false;     // ADD: Flag for Fake AP Menu
uint8_t fakeAPIndex = 0;        // ADD: Index for Fake AP Menu
bool fakeAPEnabled = false;    // ADD: Flag to track Fake AP status - initially off


// Captive Portal variables - Definitions (memory allocation)
DNSServer dnsServer;
ESP8266WebServer webServer;

// Fake AP settings - Definitions (memory allocation)
const char* apSSID = "Fake_AP_123";
const char* apPassword = "";
const char* captivePortalPage = "<html><body><h1>Welcome to the Fake AP!</h1><p>You are now connected to the Fake AP.</p></body></html>";

// OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Function Prototypes (for functions remaining in main.cpp)
void showMenu(uint8_t selectedIndex);
void handleMenuNavigation();
void showSettings();


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

  showMenu(selectedIndex);  // Show menu at startup
}

// Loop Function
void loop() {
  handleMenuNavigation();
  handleClientRequest(); // Handle captive portal and client requests from wifi.cpp
}

// Function to display the main menu
void showMenu(uint8_t selectedIndex) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  const char* menuOptions[5] = { "WiFi", "Radio", "RFID", "Remote", "Setting" };

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

// Function to handle menu navigation with buttons
// Function to handle menu navigation with buttons
void handleMenuNavigation() {
  static uint8_t lastStateNext = HIGH;
  static uint8_t lastStateSelect = HIGH;

  uint8_t currentStateNext = digitalRead(BUTTON_NEXT);  // Scroll button (D5)
  uint8_t currentStateSelect = digitalRead(BUTTON_SELECT);

  if (currentStateNext == LOW && lastStateNext == HIGH) {
    Serial.println("Next button pressed (LOW)");
    if (inSettings) {
      Serial.println("In Settings Menu");
      settingIndex = (settingIndex + 1) % 4;
      showSettings();
    } else if (inWiFiMenu) {
      Serial.println("In WiFi Menu - Scrolling");
      if (inFakeAPMenu) {            // ADD: Check if in Fake AP Menu
        Serial.println("In Fake AP Menu - Scrolling");
        fakeAPIndex = (fakeAPIndex + 1) % 3;
        showFakeAPMenu();
      } else {                        // Otherwise scroll WiFi Menu Options
        wifiIndex = (wifiIndex + 1) % 3; // Assuming 3 WiFi options now, adjust as needed in wifi.cpp
        showWiFiMenu();
      }
    } else {
      Serial.println("In Main Menu - Scrolling Main Options");
      selectedIndex = (selectedIndex + 1) % 5;
      showMenu(selectedIndex);
    }
    delay(200); // Debounce delay
  }

  if (currentStateSelect == LOW && lastStateSelect == HIGH) {
    Serial.println("Select button pressed (LOW) in Main Menu Handler");
    Serial.print("Current selectedIndex: "); Serial.println(selectedIndex);
    Serial.print("Current inSettings: "); Serial.println(inSettings);
    Serial.print("Current inWiFiMenu: "); Serial.println(inWiFiMenu);
    Serial.print("Current inFakeAPMenu: "); Serial.println(inFakeAPMenu); // ADD: Debug print

    if (inSettings) {
      if (settingIndex == 3) {
        inSettings = false;
        showMenu(selectedIndex);
        Serial.println("Going back to Main Menu from Settings");
      } else {
        Serial.print("Settings Option: ");
        Serial.println(settingIndex);
      }
    } else if (inWiFiMenu) {
      if (inFakeAPMenu) {        // ADD: Check if in Fake AP Menu
        Serial.println("In Fake AP Menu - Handling Selection");
        handleFakeAPMenuNavigation(); // Handle Fake AP Menu selections
        Serial.println("Returned from handleFakeAPMenuNavigation()");
      } else {                      // Otherwise handle WiFi Menu selections
        Serial.println("In WiFi Menu - Handling WiFi Select");
        handleWiFiMenuNavigation();    // Handle WiFi Menu selections
        Serial.println("Returned from handleWiFiMenuNavigation()");
      }
    } else {
      switch (selectedIndex) {
        case 0:
          inWiFiMenu = true;
          showWiFiMenu();
          Serial.println("Entering WiFi Menu from Main Menu");
          break;
        case 1: Serial.println("Radio"); break;
        case 2: Serial.println("RFID"); break;
        case 3: Serial.println("Remote"); break;
        case 4:
          inSettings = true;
          showSettings();
          Serial.println("Entering Settings Menu from Main Menu");
          break;
      }
    }
    delay(200); // Debounce delay
  }

  lastStateNext = currentStateNext;
  lastStateSelect = currentStateSelect;
}

// Function to display the Settings screen
void showSettings() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10, 10);
  display.println("Settings");

  const char* settingOptions[4] = { "Option 1", "Option 2", "Option 3", "Back" };

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