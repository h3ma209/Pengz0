#ifndef PORTALS_H
#define PORTALS_H

#include <Arduino.h>

extern int portalIndex;
extern const char *IQcaptivePortalPage;
extern const char *KOMARcaptivePortalPage;
extern const char *MYKOMARcaptivePortalPage;
extern const char *KOMARcapPortalPage;

const char *portalPageForIndex(int index);

#endif
