#ifndef CGI_H
#define CGI_H

#include "httpd.h"
#include "user_config.h"

int cgiLedRGB(HttpdConnData *connData);
int tplLed(HttpdConnData *connData, char *token, void **arg);
int tplCounter(HttpdConnData *connData, char *token, void **arg);


#endif
