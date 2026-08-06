#include "portals.h"

int portalIndex = 0;

const char *IQcaptivePortalPage =
    "<!DOCTYPE html><html><head><title>IQ Portal</title></head>"
    "<body><h1>Welcome to My IQ Portal!</h1>"
    "<p>Connect to access the internet.</p></body></html>";

const char *KOMARcaptivePortalPage =
    "<!DOCTYPE html><html><head><title>Komar Portal</title></head>"
    "<body><h1>Welcome to My Komar Portal!</h1>"
    "<p>Connect to access the internet.</p></body></html>";

const char *MYKOMARcaptivePortalPage =
    "<!DOCTYPE html><html><head><title>MyKomar Portal</title></head>"
    "<body><h1>Welcome to Komar wifi Portal!</h1>"
    "<p>Connect to access the internet.</p></body></html>";

const char *KOMARcapPortalPage =
    "<!DOCTYPE html><html><head><title>Komar Cap</title></head>"
    "<body><h1>Welcome to Komar Cap Portal!</h1>"
    "<p>Connect to access the internet.</p></body></html>";

const char *portalPageForIndex(int index) {
  switch (index) {
    case 0: return IQcaptivePortalPage;
    case 1: return KOMARcaptivePortalPage;
    case 2: return MYKOMARcaptivePortalPage;
    case 3: return KOMARcapPortalPage;
    default: return IQcaptivePortalPage;
  }
}
