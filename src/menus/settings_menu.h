// src/menus/settings_menu.h
#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

void showSettings(Adafruit_SSD1306 &display);
void handleSettingsNavigation(Adafruit_SSD1306 &display);

#endif