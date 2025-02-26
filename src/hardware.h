#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h> // Include Arduino.h to use uint8_t etc.

// Button Pins - DEFINITIONS WITH NUMBERS NOW IN HARDWARE.H
extern const uint8_t BUTTON_NEXT  = 14;  // Scroll button (e.g., connected to GPIO14) D5
extern const uint8_t BUTTON_SELECT = 12; // Select button (e.g., connected to GPIO12) D6

// You can add function prototypes related to buttons or other hardware here
// if you start moving button logic or hardware interaction functions to separate files.

#endif // HARDWARE_H