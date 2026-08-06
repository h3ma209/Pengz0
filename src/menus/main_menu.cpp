#include "main_menu.h"
#include "../app_state.h"
#include "../buttons.h"
#include "../hardware.h"
#include "settings_menu.h"
#include "wifi_menu.h"

void showMainMenu(uint8_t selectedIndex, Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.setTextSize(1);

  static const char *const options[] = {"WiFi", "Radio", "RFID", "Remote", "Setting"};
  static const uint8_t count = 5;

  for (uint8_t i = 0; i < count; i++) {
    if (i == selectedIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(10, 10 + (i * 10));
    display.println(options[i]);
  }

  display.display();
}

void handleMainMenuNavigation(uint8_t &selectedIndex, bool &inSettings, bool &inWiFiMenu,
                              Adafruit_SSD1306 &display) {
  static uint8_t lastNext = HIGH;
  static uint8_t lastSelect = HIGH;

  if (buttonEdge(BUTTON_NEXT, lastNext)) {
    selectedIndex = (selectedIndex + 1) % 5;
    showMainMenu(selectedIndex, display);
    delay(BUTTON_DEBOUNCE_MS);
  }

  if (buttonEdge(BUTTON_SELECT, lastSelect)) {
    switch (selectedIndex) {
      case 0:
        inMainMenu = false;
        inWiFiMenu = true;
        showWiFiMenu(display);
        break;
      case 1:
      case 2:
      case 3:
        // Placeholders
        break;
      case 4:
        inMainMenu = false;
        inSettings = true;
        settingIndex = 0;
        showSettings(display);
        break;
    }
    delay(BUTTON_DEBOUNCE_MS);
  }
}
