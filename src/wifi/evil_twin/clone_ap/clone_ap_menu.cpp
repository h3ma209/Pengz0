#include "clone_ap_menu.h"
#include "clone_ap.h"
#include "../../../app_state.h"
#include "../../../buttons.h"
#include "../../../hardware.h"
#include "../../fake_ap/fake_ap.h"
#include "../evil_twin_menu.h"

static const uint8_t VISIBLE_ROWS = 4;

void showCloneAPNetworkMenu(Adafruit_SSD1306 &display) {
  if (scannedNetworks.empty()) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println(F("Scanning..."));
    display.display();

    displayKawaskiBitmap();
    scanWiFiNetworks(scannedNetworks);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Clone AP"));

  if (scannedNetworks.empty()) {
    display.setCursor(0, 16);
    display.println(F("No networks found"));
    display.setCursor(0, 28);
    display.println(F("> Back"));
    display.display();
    return;
  }

  const size_t itemCount = scannedNetworks.size() + 1; // + Back
  size_t start = 0;
  if (cloneAPIndex >= VISIBLE_ROWS) {
    start = cloneAPIndex - VISIBLE_ROWS + 1;
  }
  if (start + VISIBLE_ROWS > itemCount) {
    start = (itemCount > VISIBLE_ROWS) ? (itemCount - VISIBLE_ROWS) : 0;
  }

  for (uint8_t row = 0; row < VISIBLE_ROWS; ++row) {
    size_t i = start + row;
    if (i >= itemCount) {
      break;
    }

    if (i == cloneAPIndex) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }

    display.setCursor(0, 12 + (row * 12));
    if (i < scannedNetworks.size()) {
      display.println(scannedNetworks[i].ssid);
    } else {
      display.println(F("Back"));
    }
  }

  display.display();
}

void handleCloneAPNetworkMenuNavigation(Adafruit_SSD1306 &display) {
  static uint8_t lastNext = HIGH;
  static uint8_t lastSelect = HIGH;

  const size_t itemCount = scannedNetworks.empty() ? 1 : (scannedNetworks.size() + 1);

  if (buttonEdge(BUTTON_NEXT, lastNext)) {
    cloneAPIndex = (cloneAPIndex + 1) % itemCount;
    showCloneAPNetworkMenu(display);
    delay(BUTTON_DEBOUNCE_MS);
  }

  if (buttonEdge(BUTTON_SELECT, lastSelect)) {
    if (scannedNetworks.empty() || cloneAPIndex == scannedNetworks.size()) {
      inCloneAPMenu = false;
      inEvilTwinMenu = true;
      scannedNetworks.clear();
      cloneAPIndex = 0;
      showEvilTwinMenu(display);
    } else {
      cloneWiFiNetwork(scannedNetworks[cloneAPIndex]);
      inCloneAPMenu = false;
      inEvilTwinMenu = true;
      cloneAPIndex = 0;
      showEvilTwinMenu(display);
    }
    delay(BUTTON_DEBOUNCE_MS);
  }
}
