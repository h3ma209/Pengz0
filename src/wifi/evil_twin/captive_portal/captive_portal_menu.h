#ifndef CAPTIVE_PORTAL_MENU_H
#define CAPTIVE_PORTAL_MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

void showCaptivePortalMenu(Adafruit_SSD1306 &display);
void handleCaptivePortalMenuNavigation(Adafruit_SSD1306 &display);

#endif