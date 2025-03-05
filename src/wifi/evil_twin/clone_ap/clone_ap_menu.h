// src/wifi/evil_twin/clone_ap/clone_ap_menu.h
#ifndef CLONE_AP_MENU_H
#define CLONE_AP_MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

void showCloneAPNetworkMenu(Adafruit_SSD1306 &display);
void handleCloneAPNetworkMenuNavigation(Adafruit_SSD1306 &display);
void scanForNetworks();
#endif