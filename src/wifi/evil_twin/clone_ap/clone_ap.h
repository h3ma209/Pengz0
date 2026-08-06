#ifndef CLONE_AP_H
#define CLONE_AP_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <vector>

struct WiFiNetwork {
  String ssid;
  String bssid;
  int32_t rssi;
  int32_t encryptionType;
};

bool scanWiFiNetworks(std::vector<WiFiNetwork> &networks);
void printScannedNetworks(const std::vector<WiFiNetwork> &networks);
bool cloneWiFiNetwork(const WiFiNetwork &networkToClone);
void setAPSSID(String ssid);

#endif
