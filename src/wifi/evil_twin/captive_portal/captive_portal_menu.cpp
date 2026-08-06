#include "captive_portal_menu.h"
#include "../../../app_state.h"
#include "../../../buttons.h"
#include "../../../hardware.h"
#include "../evil_twin_menu.h"
#include "portals/portals.h"

void showCaptivePortalMenu(Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(F("Portal #"));
  display.println(portalIndex);

  static const char *const options[] = {"IQ Login", "Google Login", "MyKomar", "Komar Cap", "Back"};
  for (uint8_t i = 0; i < 5; i++) {
    if (i == captivePortalIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(0, 12 + (i * 10));
    if (i < 4 && i == portalIndex) {
      display.print(F("*"));
    } else {
      display.print(F(" "));
    }
    display.println(options[i]);
  }
  display.display();
}

void handleCaptivePortalMenuNavigation(Adafruit_SSD1306 &display) {
  static uint8_t lastNext = HIGH;
  static uint8_t lastSelect = HIGH;

  if (buttonEdge(BUTTON_NEXT, lastNext)) {
    captivePortalIndex = (captivePortalIndex + 1) % 5;
    showCaptivePortalMenu(display);
    delay(BUTTON_DEBOUNCE_MS);
  }

  if (buttonEdge(BUTTON_SELECT, lastSelect)) {
    if (captivePortalIndex == 4) {
      inCaptivePortalMenu = false;
      inEvilTwinMenu = true;
      captivePortalIndex = 0;
      showEvilTwinMenu(display);
    } else {
      portalIndex = captivePortalIndex;
      showCaptivePortalMenu(display);
    }
    delay(BUTTON_DEBOUNCE_MS);
  }
}
