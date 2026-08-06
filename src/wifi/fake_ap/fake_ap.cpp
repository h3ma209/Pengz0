#include "fake_ap.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <Adafruit_SSD1306.h>

#include "../../app_state.h"
#include "../../bitmaps/beegyoshi.h"
#include "../../bitmaps/kawaski.h"
#include "../evil_twin/captive_portal/portals/portals.h"

static String currentPortalHtml;
static IPAddress apIP(192, 168, 1, 1);

static void handleRoot() {
  webServer.send(200, "text/html", currentPortalHtml);
}

void displayBeegYoshiBitmap() {
  display.clearDisplay();
  display.drawBitmap(0, 0, beegyoshiBitmap, 128, 64, SSD1306_WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
}

void displayKawaskiBitmap() {
  display.clearDisplay();
  display.drawBitmap(0, 0, kawaskiBitmap, 128, 64, SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(64, 24);
  display.println(F("Kawaski"));
  display.setCursor(64, display.getCursorY());
  display.println(F("Analysis"));
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
}

void startFakeAP() {
  if (fakeAPEnabled) {
    return;
  }

  displayKawaskiBitmap();

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID, apPassword);

  dnsServer.start(53, "*", apIP);

  currentPortalHtml = portalPageForIndex(portalIndex);
  if (currentPortalHtml.length() == 0) {
    currentPortalHtml = captivePortalPage;
  }

  webServer.on("/", handleRoot);
  webServer.on("/generate_204", handleRoot);
  webServer.on("/hotspot-detect.html", handleRoot);
  webServer.on("/redirect", handleRoot);
  webServer.onNotFound(handleRoot);
  webServer.begin();

  fakeAPEnabled = true;
  Serial.print(F("Fake AP started: "));
  Serial.println(apSSID);
}

void loopAP() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

void stopFakeAP() {
  if (!fakeAPEnabled) {
    return;
  }

  displayKawaskiBitmap();
  webServer.stop();
  dnsServer.stop();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);
  fakeAPEnabled = false;
  Serial.println(F("Fake AP stopped"));
}
