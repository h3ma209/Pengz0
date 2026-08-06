#include "wifi_menu.h"
#include "../app_state.h"
#include "../buttons.h"
#include "../hardware.h"
#include "../wifi/fake_ap/fake_ap_menu.h"
#include "../wifi/evil_twin/evil_twin_menu.h"
#include "main_menu.h"

void showWiFiMenu(Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("WiFi Menu"));

  static const char *const options[] = {"Fake AP", "Evil Twin", "Scan (TBD)", "Back"};
  for (uint8_t i = 0; i < 4; i++) {
    if (i == wifiIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(10, 14 + (i * 12));
    display.println(options[i]);
  }
  display.display();
}

void handleWiFiMenuNavigation(Adafruit_SSD1306 &display) {
  static uint8_t lastNext = HIGH;
  static uint8_t lastSelect = HIGH;

  if (buttonEdge(BUTTON_NEXT, lastNext)) {
    wifiIndex = (wifiIndex + 1) % 4;
    showWiFiMenu(display);
    delay(BUTTON_DEBOUNCE_MS);
  }

  if (buttonEdge(BUTTON_SELECT, lastSelect)) {
    switch (wifiIndex) {
      case 0:
        inWiFiMenu = false;
        inFakeAPMenu = true;
        fakeAPIndex = 0;
        showFakeAPMenu(display);
        break;
      case 1:
        inWiFiMenu = false;
        inEvilTwinMenu = true;
        evilTwinIndex = 0;
        showEvilTwinMenu(display);
        break;
      case 2:
        // TBD
        break;
      case 3:
        inWiFiMenu = false;
        inMainMenu = true;
        wifiIndex = 0;
        showMainMenu(selectedIndex, display);
        break;
    }
    delay(BUTTON_DEBOUNCE_MS);
  }
}
