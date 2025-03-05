// src/menus/main_menu.h
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

// Function prototypes for Main Menu
void showMainMenu(uint8_t selectedIndex, Adafruit_SSD1306 &display); // Pass display as reference
void handleMainMenuNavigation(uint8_t &selectedIndex, bool &inSettings, bool &inWiFiMenu, Adafruit_SSD1306 &display); // Pass state variables as references

#endif // MAIN_MENU_H