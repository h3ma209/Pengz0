#include "fake_ap_menu.h"
#include "../../app_state.h"
#include "../../buttons.h"
#include "../../hardware.h"
#include "../../menus/wifi_menu.h"
#include "fake_ap.h"

void showFakeAPMenu(Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print(F("Status: "));
  display.println(fakeAPEnabled ? F("ON") : F("OFF"));

  display.setCursor(0, 10);
  display.print(F("SSID: "));
  display.println(apSSID);

  static const char *const options[] = {"Turn On", "Turn Off", "Back"};
  for (uint8_t i = 0; i < 3; i++) {
    if (i == fakeAPIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(0, 24 + (i * 12));
    display.println(options[i]);
  }
  display.display();
}

void handleFakeAPMenuNavigation(Adafruit_SSD1306 &display) {
  static uint8_t lastNext = HIGH;
  static uint8_t lastSelect = HIGH;

  if (buttonEdge(BUTTON_NEXT, lastNext)) {
    fakeAPIndex = (fakeAPIndex + 1) % 3;
    showFakeAPMenu(display);
    delay(BUTTON_DEBOUNCE_MS);
  }

  if (buttonEdge(BUTTON_SELECT, lastSelect)) {
    switch (fakeAPIndex) {
      case 0:
        startFakeAP();
        showFakeAPMenu(display);
        break;
      case 1:
        stopFakeAP();
        showFakeAPMenu(display);
        break;
      case 2:
        inFakeAPMenu = false;
        inWiFiMenu = true;
        fakeAPIndex = 0;
        showWiFiMenu(display);
        break;
    }
    delay(BUTTON_DEBOUNCE_MS);
  }
}
