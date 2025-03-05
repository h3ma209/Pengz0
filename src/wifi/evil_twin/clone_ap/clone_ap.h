// src/wifi/evil_twin/clone_ap/clone_ap.h
#ifndef CLONE_AP_H
#define CLONE_AP_H

#include <ESP8266WiFi.h>
#include <vector> // For using std::vector to store scanned networks
#include <Arduino.h>

// Struct to hold information about a scanned WiFi network
struct WiFiNetwork {
    String ssid;
    String bssid; // BSSID (MAC Address) as String for easy display
    int32_t rssi;
    int32_t encryptionType; // You might want to store encryption type if needed
};

// Function prototypes
bool scanWiFiNetworks(std::vector<WiFiNetwork> &networks);
void printScannedNetworks(const std::vector<WiFiNetwork> &networks); // For debugging - print to Serial
bool cloneWiFiNetwork(const WiFiNetwork &networkToClone); // Cloning function prototype
void setAPSSID(String ssid); // Set the SSID for the cloned AP


#endif // CLONE_AP_H