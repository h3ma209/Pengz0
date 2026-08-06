#include "clone_ap.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <string.h>
#include "../../../app_state.h"

bool scanWiFiNetworks(std::vector<WiFiNetwork> &networks) {
  networks.clear();
  Serial.println(F("Scanning WiFi..."));

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  if (n <= 0) {
    Serial.println(F("No networks found"));
    return false;
  }

  for (int i = 0; i < n; ++i) {
    WiFiNetwork network;
    network.ssid = WiFi.SSID(i);
    network.bssid = WiFi.BSSIDstr(i);
    network.rssi = WiFi.RSSI(i);
    network.encryptionType = WiFi.encryptionType(i);
    networks.push_back(network);
  }

  Serial.print(n);
  Serial.println(F(" networks found"));
  return true;
}

void printScannedNetworks(const std::vector<WiFiNetwork> &networks) {
  for (size_t i = 0; i < networks.size(); ++i) {
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.print(networks[i].ssid);
    Serial.print(F(" RSSI="));
    Serial.println(networks[i].rssi);
  }
}

bool cloneWiFiNetwork(const WiFiNetwork &networkToClone) {
  Serial.print(F("Clone SSID -> "));
  Serial.println(networkToClone.ssid);
  setAPSSID(networkToClone.ssid);
  return true;
}

void setAPSSID(String ssid) {
  strncpy(apSSID, ssid.c_str(), sizeof(apSSID) - 1);
  apSSID[sizeof(apSSID) - 1] = '\0';
}
