#ifndef WIFI_MENU_H
#define WIFI_MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

void showWiFiMenu(Adafruit_SSD1306 &display);
void handleWiFiMenuNavigation(Adafruit_SSD1306 &display);

#endif