#ifndef CGI_H
#define CGI_H

#include "httpd.h"
#include "user_config.h"

extern char red_led;
extern char green_led;
extern char blue_led;

int cgiLedRGB(HttpdConnData *connData);
int tplLed(HttpdConnData *connData, char *token, void **arg);
int tplCounter(HttpdConnData *connData, char *token, void **arg);


#endif
