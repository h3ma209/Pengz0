#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_SSD1306.h>
#include <vector>

#include "hardware.h"
#include "app_state.h"
#include "menus/main_menu.h"
#include "menus/settings_menu.h"
#include "menus/wifi_menu.h"
#include "wifi/fake_ap/fake_ap.h"
#include "wifi/fake_ap/fake_ap_menu.h"
#include "wifi/evil_twin/evil_twin_menu.h"
#include "wifi/evil_twin/clone_ap/clone_ap_menu.h"
#include "wifi/evil_twin/captive_portal/captive_portal_menu.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const uint8_t BUTTON_NEXT = 14;   // D5
const uint8_t BUTTON_SELECT = 12; // D6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool inMainMenu = true;
bool inSettings = false;
bool inWiFiMenu = false;
bool inFakeAPMenu = false;
bool inEvilTwinMenu = false;
bool inCloneAPMenu = false;
bool inCaptivePortalMenu = false;

uint8_t selectedIndex = 0;
uint8_t settingIndex = 0;
uint8_t wifiIndex = 0;
uint8_t fakeAPIndex = 0;
uint8_t evilTwinIndex = 0;
uint8_t cloneAPIndex = 0;
uint8_t captivePortalIndex = 0;

bool fakeAPEnabled = false;
DNSServer dnsServer;
ESP8266WebServer webServer(80);
char apSSID[50] = "FakeAP";
const char *apPassword = "";
String captivePortalPage =
    "<html><body><h1>Welcome to the Fake AP!</h1>"
    "<p>You are now connected to the Fake AP.</p></body></html>";

std::vector<WiFiNetwork> scannedNetworks;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) {
      delay(1000);
    }
  }

  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);

  WiFi.mode(WIFI_OFF);
  showMainMenu(selectedIndex, display);

  Serial.println(F("Setup complete"));
}

void loop() {
  if (inMainMenu) {
    handleMainMenuNavigation(selectedIndex, inSettings, inWiFiMenu, display);
  } else if (inSettings) {
    handleSettingsNavigation(display);
  } else if (inWiFiMenu) {
    handleWiFiMenuNavigation(display);
  } else if (inFakeAPMenu) {
    handleFakeAPMenuNavigation(display);
  } else if (inEvilTwinMenu) {
    handleEvilTwinMenuNavigation(display);
  } else if (inCloneAPMenu) {
    handleCloneAPNetworkMenuNavigation(display);
  } else if (inCaptivePortalMenu) {
    handleCaptivePortalMenuNavigation(display);
  }

  if (fakeAPEnabled) {
    loopAP();
  }
}
