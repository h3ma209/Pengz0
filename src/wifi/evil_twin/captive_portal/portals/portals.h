#ifndef PORTALS_H
#define PORTALS_H

#include <Arduino.h>

// String captivePortalPage = "<html><body><h1>Welcome to the Fake AP!</h1><p>You are now connected to the Fake AP.</p></body></html>";
// make a string that will be used as the captive portal page
extern int portalIndex = 0;
extern String IQcaptivePortalPage PROGMEM = "<!DOCTYPE html>\n<html>\n<head>\n    <title>My Captive Portal</title>\n</head>\n<body>\n    <h1>Welcome to My IQ Portal!</h1>\n    <img src=\"data:image/png;base64,iVBORw0K...very_short_base64...g==\" alt=\"My Logo\">\n    <p>Connect to access the internet.</p>\n</body>\n</html>\n";
extern String KOMARcaptivePortalPage PROGMEM= "<!DOCTYPE html>\n<html>\n<head>\n    <title>My Captive Portal</title>\n</head>\n<body>\n    <h1>Welcome to My Komar Portal!</h1>\n    <img src=\"data:image/png;base64,iVBORw0K...very_short_base64...g==\" alt=\"My Logo\">\n    <p>Connect to access the internet.</p>\n</body>\n</html>\n";
extern String MYKOMARcaptivePortalPage PROGMEM= "<!DOCTYPE html>\n<html>\n<head>\n    <title>My Captive Portal</title>\n</head>\n<body>\n    <h1>Welcome to Komar wifi Portal!</h1>\n    <img src=\"data:image/png;base64,iVBORw0K...very_short_base64...g==\" alt=\"My Logo\">\n    <p>Connect to access the internet.</p>\n</body>\n</html>\n";

#endif // PORTALS_H
