#include "settings_menu.h"
#include "../app_state.h"
#include "../buttons.h"
#include "../hardware.h"
#include "main_menu.h"

void showSettings(Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("Settings"));

  static const char *const options[] = {"Option 1", "Option 2", "Option 3", "Back"};
  for (uint8_t i = 0; i < 4; i++) {
    if (i == settingIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(10, 14 + (i * 12));
    display.println(options[i]);
  }
  display.display();
}

void handleSettingsNavigation(Adafruit_SSD1306 &display) {
  static uint8_t lastNext = HIGH;
  static uint8_t lastSelect = HIGH;

  if (buttonEdge(BUTTON_NEXT, lastNext)) {
    settingIndex = (settingIndex + 1) % 4;
    showSettings(display);
    delay(BUTTON_DEBOUNCE_MS);
  }

  if (buttonEdge(BUTTON_SELECT, lastSelect)) {
    if (settingIndex == 3) {
      inSettings = false;
      inMainMenu = true;
      settingIndex = 0;
      showMainMenu(selectedIndex, display);
    }
    delay(BUTTON_DEBOUNCE_MS);
  }
}
