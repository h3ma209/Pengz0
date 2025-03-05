// src/wifi/evil_twin/evil_twin_menu.h
#ifndef EVIL_TWIN_MENU_H
#define EVIL_TWIN_MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

void showEvilTwinMenu(Adafruit_SSD1306 &display);
void handleEvilTwinMenuNavigation(Adafruit_SSD1306 &display);

#endif