// src/menus/fake_ap_menu.h
#ifndef FAKE_AP_MENU_H
#define FAKE_AP_MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

void showFakeAPMenu(Adafruit_SSD1306 &display);
void handleFakeAPMenuNavigation(Adafruit_SSD1306 &display);

#endif