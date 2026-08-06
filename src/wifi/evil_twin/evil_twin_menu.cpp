#include "evil_twin_menu.h"
#include "../../app_state.h"
#include "../../buttons.h"
#include "../../hardware.h"
#include "../../menus/wifi_menu.h"
#include "clone_ap/clone_ap_menu.h"
#include "captive_portal/captive_portal_menu.h"

void showEvilTwinMenu(Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Evil Twin"));

  static const char *const options[] = {"Clone AP", "Captive Portal", "Back"};
  for (uint8_t i = 0; i < 3; i++) {
    if (i == evilTwinIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(0, 14 + (i * 12));
    display.println(options[i]);
  }
  display.display();
}

void handleEvilTwinMenuNavigation(Adafruit_SSD1306 &display) {
  static uint8_t lastNext = HIGH;
  static uint8_t lastSelect = HIGH;

  if (buttonEdge(BUTTON_NEXT, lastNext)) {
    evilTwinIndex = (evilTwinIndex + 1) % 3;
    showEvilTwinMenu(display);
    delay(BUTTON_DEBOUNCE_MS);
  }

  if (buttonEdge(BUTTON_SELECT, lastSelect)) {
    switch (evilTwinIndex) {
      case 0:
        inEvilTwinMenu = false;
        inCloneAPMenu = true;
        cloneAPIndex = 0;
        showCloneAPNetworkMenu(display);
        break;
      case 1:
        inEvilTwinMenu = false;
        inCaptivePortalMenu = true;
        captivePortalIndex = 0;
        showCaptivePortalMenu(display);
        break;
      case 2:
        inEvilTwinMenu = false;
        inWiFiMenu = true;
        evilTwinIndex = 0;
        showWiFiMenu(display);
        break;
    }
    delay(BUTTON_DEBOUNCE_MS);
  }
}
