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

// OS probes these URLs. Wrong reply → "Connected, no internet".
// Correct success reply → OS thinks online (portal still at /).

static void handleRoot() {
  webServer.send(200, "text/html", currentPortalHtml);
}

static void handleAndroid204() {
  webServer.send(204, "text/plain", "");
}

static void handleAppleSuccess() {
  webServer.send(200, "text/html",
                 F("<HTML><HEAD><TITLE>Success</TITLE></HEAD>"
                   "<BODY>Success</BODY></HTML>"));
}

static void handleWindowsNCSI() {
  webServer.send(200, "text/plain", F("Microsoft Connect Test"));
}

static void handleWindowsNCSIAlt() {
  webServer.send(200, "text/plain", F("Microsoft NCSI"));
}

static void handleConnectivityOrPortal() {
  const String &uri = webServer.uri();

  if (uri.indexOf(F("generate_204")) >= 0 || uri.indexOf(F("gen_204")) >= 0 ||
      uri.indexOf(F("connecttest")) >= 0 || uri.indexOf(F("ncsi")) >= 0 ||
      uri.indexOf(F("hotspot-detect")) >= 0 || uri.indexOf(F("success.txt")) >= 0 ||
      uri.indexOf(F("canonical.html")) >= 0 || uri.indexOf(F("kindle-wifi")) >= 0) {
    if (uri.indexOf(F("hotspot-detect")) >= 0 || uri.indexOf(F("canonical.html")) >= 0) {
      handleAppleSuccess();
    } else if (uri.indexOf(F("connecttest")) >= 0) {
      handleWindowsNCSI();
    } else if (uri.indexOf(F("ncsi")) >= 0) {
      handleWindowsNCSIAlt();
    } else {
      handleAndroid204();
    }
    return;
  }

  handleRoot();
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

  // Captive portal page (open http://192.168.1.1/ manually)
  webServer.on("/", handleRoot);

  // Android
  webServer.on("/generate_204", handleAndroid204);
  webServer.on("/gen_204", handleAndroid204);

  // Apple / iOS
  webServer.on("/hotspot-detect.html", handleAppleSuccess);
  webServer.on("/library/test/success.html", handleAppleSuccess);

  // Windows
  webServer.on("/connecttest.txt", handleWindowsNCSI);
  webServer.on("/ncsi.txt", handleWindowsNCSIAlt);
  webServer.on("/redirect", handleAndroid204);

  // Catch-all: spoof known probes, else show portal
  webServer.onNotFound(handleConnectivityOrPortal);
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
