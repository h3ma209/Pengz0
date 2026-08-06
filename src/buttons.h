#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "hardware.h"

// Rising-edge press on INPUT_PULLUP button (active LOW).
// Updates lastState. Caller may debounce with delay after handling.
inline bool buttonEdge(uint8_t pin, uint8_t &lastState) {
  uint8_t current = digitalRead(pin);
  bool pressed = (current == LOW && lastState == HIGH);
  lastState = current;
  return pressed;
}

static const uint16_t BUTTON_DEBOUNCE_MS = 180;

#endif
