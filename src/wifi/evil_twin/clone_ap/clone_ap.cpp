// src/wifi/evil_twin/clone_ap/clone_ap.cpp
#include "clone_ap.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <vector>

// Function to scan for WiFi networks
bool scanWiFiNetworks(std::vector<WiFiNetwork> &networks) {
    Serial.println("Starting WiFi scan...");
    networks.clear(); // Clear any previous scan results

    int n = WiFi.scanNetworks(); // Returns the number of networks found
    if (n == 0) {
        Serial.println("No WiFi networks found.");
        return false; // Scan failed (no networks found is also considered a failure for this example)
    } else {
        Serial.print(n);
        Serial.println(" networks found:");
        for (int i = 0; i < n; ++i) {
            WiFiNetwork network;
            network.ssid = WiFi.SSID(i);
            network.bssid = WiFi.BSSIDstr(i); // Get BSSID (MAC address) as string
            network.rssi = WiFi.RSSI(i);
            network.encryptionType = WiFi.encryptionType(i);

            networks.push_back(network); // Add the network info to the vector

            // For debugging, print to serial:
            Serial.print(i + 1);
            Serial.print(": SSID: ");
            Serial.print(network.ssid);
            Serial.print(" | BSSID: ");
            Serial.print(network.bssid);
            Serial.print(" | RSSI: ");
            Serial.print(network.rssi);
            Serial.print(" | Encryption: ");
            Serial.println(network.encryptionType); // You can decode encryption type if needed later
            delay(100); // ചെറിയ delay കൊടുക്കുന്നത് நல்லது serial print-ന്
        }
        return true; // Scan successful
    }
}

// Function to print scanned networks (for debugging)
void printScannedNetworks(const std::vector<WiFiNetwork> &networks) {
    Serial.println("\n----- Scanned WiFi Networks -----");
    if (networks.empty()) {
        Serial.println("No networks in the list.");
    } else {
        for (size_t i = 0; i < networks.size(); ++i) {
            Serial.print(i + 1);
            Serial.print(": SSID: ");
            Serial.print(networks[i].ssid);
            Serial.print(" | BSSID: ");
            Serial.print(networks[i].bssid);
            Serial.print(" | RSSI: ");
            Serial.print(networks[i].rssi);
            Serial.print(" | Encryption: ");
            Serial.println(networks[i].encryptionType);
        }
    }
    Serial.println("----- End of Scan List -----");
}


// Placeholder function for cloning a WiFi network - IMPLEMENTATION PENDING
bool cloneWiFiNetwork(const WiFiNetwork &networkToClone) {
    Serial.print("cloneWiFiNetwork() called for SSID: ");
    Serial.println(networkToClone.ssid);
    Serial.print("BSSID: ");
    Serial.println(networkToClone.bssid);
    // ... Cloning logic will be added here later ...
    return false; // Placeholder - cloning not implemented yet
}