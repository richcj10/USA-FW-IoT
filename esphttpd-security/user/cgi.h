#ifndef CGI_H
#define CGI_H

#include "httpd.h"
#include "user_config.h"

extern int red_led;
extern int green_led;
extern int blue_led;

int cgiLedRGB(HttpdConnData *connData);
int tplLed(HttpdConnData *connData, char *token, void **arg);
int tplCounter(HttpdConnData *connData, char *token, void **arg);


#endif
